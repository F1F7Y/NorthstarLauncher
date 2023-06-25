#include "core/convar/convar.h"
#include "core/convar/concommand.h"
#include "mods/modmanager.h"
#include "core/tier0.h"
#include "engine/r2engine.h"

#include "core/math/vector.h"
#include <vector>

// Triggers defined in .ent files don't use bsp models, hence they dont have drawable geometry generated
// which is why `showtriggers` doesn't work for them.
// We hook the ent trigger kv parsing func and build our own meshes at runtime. Then use debug overlay to draw them.

//-----------------------------------------------------------------------------
// Render func prototypes

// Render Line
typedef void (*RenderLineType)(const Vector3& v1, const Vector3& v2, Color c, bool bZBuffer);
static RenderLineType RenderLine;

// Render box
typedef void (*RenderBoxType)(
	const Vector3& vOrigin, const QAngle& angles, const Vector3& vMins, const Vector3& vMaxs, Color c, bool bZBuffer, bool bInsideOut);
static RenderBoxType RenderBox;

// Render wireframe box
static RenderBoxType RenderWireframeBox;

// Render swept box
typedef void (*RenderWireframeSweptBoxType)(
	const Vector3& vStart, const Vector3& vEnd, const QAngle& angles, const Vector3& vMins, const Vector3& vMaxs, Color c, bool bZBuffer);
static RenderWireframeSweptBoxType RenderWireframeSweptBox;

// Render Triangle
typedef void (*RenderTriangleType)(const Vector3& p1, const Vector3& p2, const Vector3& p3, Color c, bool bZBuffer);
static RenderTriangleType RenderTriangle;

// Render Axis
typedef void (*RenderAxisType)(const Vector3& vOrigin, float flScale, bool bZBuffer);
static RenderAxisType RenderAxis;

// I dont know
typedef void (*RenderUnknownType)(const Vector3& vUnk, float flUnk, bool bUnk);
static RenderUnknownType RenderUnknown;

// Render Sphere
typedef void (*RenderSphereType)(const Vector3& vCenter, float flRadius, int nTheta, int nPhi, Color c, bool bZBuffer);
static RenderSphereType RenderSphere;

//-----------------------------------------------------------------------------
// Custom trigger structures
struct Vertex_t
{
	float x, y, z;
};

struct Plane_t
{
	float a, b, c, d;
	std::vector<Vertex_t> vecVertices;
};

struct Brush_t
{
	std::vector<Plane_t> vecPlanes;
};

struct Trigger_t
{
	std::vector<Brush_t> vecBrushes;
};

//-----------------------------------------------------------------------------

std::vector<Trigger_t> vecTriggers;
bool bHasBuiltMeshes = false;
bool bShouldDrawTriggers = false;

//-----------------------------------------------------------------------------
// Mesh building

bool FloatCompare(float A, float B)
{
	return fabsf(A - B) < 0.1;
}

bool VertexCompare(const Vertex_t& vertA, const Vertex_t& vertB)
{
	return FloatCompare(vertA.x, vertB.x) && FloatCompare(vertA.y, vertB.y) && FloatCompare(vertA.z, vertB.z);
}

bool FloatInValid(float f)
{
	return isnan(f) || isinf(f) || f != f;
}

void BuildTriggerMeshes()
{
	bHasBuiltMeshes = true;

	/* vecTriggers.clear();
	Trigger_t& trigger = vecTriggers.emplace_back();
	Brush_t& brush = trigger.vecBrushes.emplace_back();
	
	//"*trigger_brush_0_plane_0" "-1 0 0 64"
	//"*trigger_brush_0_plane_1" "1 0 0 64"
	//"*trigger_brush_0_plane_2" "0 -1 0 64"
	//"*trigger_brush_0_plane_3" "0 1 0 64"
	//"*trigger_brush_0_plane_4" "0 0 -1 64"
	//"*trigger_brush_0_plane_5" "0 0 1 64"
	
	{
		Plane_t& plane = brush.vecPlanes.emplace_back();
		plane.a = -1.0f;
		plane.b = 0.0;
		plane.c = 0.0;
		plane.d = 1248.0f;
	}
	{
		Plane_t& plane = brush.vecPlanes.emplace_back();
		plane.a = 1.0f;
		plane.b = 0.0;
		plane.c = 0.0;
		plane.d = 1248.0f;
	}
	{
		Plane_t& plane = brush.vecPlanes.emplace_back();
		plane.a = 0.0f;
		plane.b = -1.0;
		plane.c = 0.0;
		plane.d = 568.0f;
	}
	{
		Plane_t& plane = brush.vecPlanes.emplace_back();
		plane.a = 0.0f;
		plane.b = 1.0;
		plane.c = 0.0;
		plane.d = 568.0f;
	}
	{
		Plane_t& plane = brush.vecPlanes.emplace_back();
		plane.a = 0.0f;
		plane.b = 0.0;
		plane.c = -1.0;
		plane.d = 336.0f;
	}
	{
		Plane_t& plane = brush.vecPlanes.emplace_back();
		plane.a = 0.0f;
		plane.b = 0.0;
		plane.c = 1.0;
		plane.d = 336.0f;
	}*/

	spdlog::info("Building meshes for ent triggers!");

	// ax + by + cz = -d
	// ax + by + cz = -d
	// ax + by + cz = -d

	// | a b c | | x | = | -d |
	// | a b c | | y | = | -d |
	// | a b c | | z | = | -d |

	for (Trigger_t& trigger : vecTriggers)
	{
		for (Brush_t& brush : trigger.vecBrushes)
		{
			for (std::size_t i = 0; i < brush.vecPlanes.size(); i++)
			{
				Plane_t& planeA = brush.vecPlanes.at(i);
				for (std::size_t j = 0; j < brush.vecPlanes.size(); j++)
				{
					if (j == i)
						continue;

					const Plane_t& planeB = brush.vecPlanes.at(j);
					for (std::size_t k = 0; k < brush.vecPlanes.size(); k++)
					{
						if (j == k || k == i)
							continue;
						const Plane_t& planeC = brush.vecPlanes.at(k);

						// Main matrix
					    float a, b, c, d, e, f, g, h, i;
						a = planeA.a;
						b = planeA.b;
						c = planeA.c;
						d = planeB.a;
						e = planeB.b;
						f = planeB.c;
						g = planeC.a;
						h = planeC.b;
						i = planeC.c;

						float fDet = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - g * e);

						// Matrix A
						a = planeA.d;
						b = planeA.b;
						c = planeA.c;
						d = planeB.d;
						e = planeB.b;
						f = planeB.c;
						g = planeC.d;
						h = planeC.b;
						i = planeC.c;

						float fDetA = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - g * e);

						// Matrix B
						a = planeA.a;
						b = planeA.d;
						c = planeA.c;
						d = planeB.a;
						e = planeB.d;
						f = planeB.c;
						g = planeC.a;
						h = planeC.d;
						i = planeC.c;

						float fDetB = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - g * e);

						// Matrix C
						a = planeA.a;
						b = planeA.b;
						c = planeA.d;
						d = planeB.a;
						e = planeB.b;
						f = planeB.d;
						g = planeC.a;
						h = planeC.b;
						i = planeC.d;

						float fDetC = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - g * e);

						// Final vertex
						Vertex_t vertex;
						vertex.x = fDetA / fDet;
						vertex.y = fDetB / fDet;
						vertex.z = fDetC / fDet;

						bool bInclude = false;
						if (FloatInValid(vertex.x), FloatInValid(vertex.y), FloatInValid(vertex.z))
							continue;

						for (const Plane_t& plane : brush.vecPlanes)
						{
							float fPos = plane.a * vertex.x + plane.b * vertex.y + plane.c + vertex.z - plane.d;
							//spdlog::error("{}", fPos);
							if (fPos > -1.001f && fPos < 1.001f)
							{
								bInclude = true;
								break;
							}
						}
						//spdlog::error("----");

						if (!bInclude)
							continue;

						bool bExists = false;
						for (const Vertex_t& vert : planeA.vecVertices)
						{
							bExists |= VertexCompare(vert, vertex);
						}

						if (!bExists)
							planeA.vecVertices.emplace_back(vertex);
					}
				}
			}
		}
	}

	spdlog::info("Finished!");
}

void DrawEntTriggers()
{
	if (!bShouldDrawTriggers)
		return;

	for (const Trigger_t& trigger : vecTriggers)
	{
		for (const Brush_t& brush : trigger.vecBrushes)
		{
			for (const Plane_t& plane : brush.vecPlanes)
			{
				if (plane.vecVertices.size() < 3)
					continue; //

				for (std::size_t i = 0; i < plane.vecVertices.size() - 2; i++)
				{

					const Vertex_t& vertexA = plane.vecVertices.at(i);
					const Vertex_t& vertexB = plane.vecVertices.at(i + 1);
					const Vertex_t& vertexC = plane.vecVertices.at(i + 2);

					Vector3 vectorA = Vector3(vertexA.x, vertexA.y, vertexA.z);
					Vector3 vectorB = Vector3(vertexB.x, vertexB.y, vertexB.z);
					Vector3 vectorC = Vector3(vertexC.x, vertexC.y, vertexC.z);

					RenderTriangle(vectorA, vectorB, vectorC, Color(250, 200, 90, 70), true);
				}
			}
			break;
		}
		break;
	}

	// Vector3 start(0.0, 0.0, 0.0);
	// RenderLine(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 255.0), 255, 0, 0, true, 5.0);
}

//-----------------------------------------------------------------------------
// ConCommand

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void ConCommand_ns_showtriggers(const CCommand& arg)
{
	if (!bHasBuiltMeshes)
		BuildTriggerMeshes();

	if (bShouldDrawTriggers)
		spdlog::info("disabled drawing ent triggers!");
	else
		spdlog::info("enabled drawing ent triggers!");

	bShouldDrawTriggers = !bShouldDrawTriggers;

	for (const Trigger_t& trigger : vecTriggers)
	{
		spdlog::warn("Trigger has {:d} brushes", trigger.vecBrushes.size());
		for (const Brush_t& brush : trigger.vecBrushes)
		{
			spdlog::warn("\tBrush has {:d} planes", brush.vecPlanes.size());
			for (const Plane_t& plane : brush.vecPlanes)
			{
				spdlog::warn("\t\tPlane {:.3f} {:.3f} {:.3f} {:.3f} has {:d} verts", plane.a, plane.b, plane.c, plane.d, plane.vecVertices.size());
				for (const Vertex_t& vert : plane.vecVertices)
				{
					spdlog::warn("\t\t\tVerT: {:.3} {:.3} {:.3}", vert.x, vert.y, vert.z);
				}
			}
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// Hooks

AUTOHOOK_INIT()

//-----------------------------------------------------------------------------
// Purpose: Gets called on map load
//-----------------------------------------------------------------------------
// clang-format off
AUTOHOOK(LoadBSP, engine.dll + 0x126930,
	bool, __fastcall, (const char* name, void* dbspinfo))
// clang-format on
{
	// Clear on map change
	vecTriggers.clear();
	bHasBuiltMeshes = false;
	return LoadBSP(name, dbspinfo);
}

//-----------------------------------------------------------------------------
// Purpose: Parses trigger key values
//-----------------------------------------------------------------------------
// clang-format off
AUTOHOOK(ParseEntTrigger, server.dll + 0x255E60,
	bool, __fastcall, (void* a1, char* szKey, char* szValue))
// clang-format on
{
	if (*szKey == '*')
	{
		// Brush / plane kv
		if (strncmp(szKey, "*trigger_brush_", sizeof("*trigger_brush_") - 1) == 0)
		{
			int iBrush, iPlane;
			sscanf(szKey, "*trigger_brush_%d_plane_%d", &iBrush, &iPlane);

			float a, b, c, d;
			sscanf(szValue, "%f %f %f %f", &a, &b, &c, &d);

			if (iBrush == 0 && iPlane == 0)
				vecTriggers.emplace_back();

			Trigger_t& trigger = vecTriggers.back();

			if (iBrush == trigger.vecBrushes.size())
				trigger.vecBrushes.emplace_back();

			Brush_t& brush = trigger.vecBrushes.back();

			Plane_t& plane = brush.vecPlanes.emplace_back();

			plane.a = a;
			plane.b = b;
			plane.c = c;
			plane.d = d;
		}
	}

	return ParseEntTrigger(a1, szKey, szValue);
}

#include <mutex>

std::mutex s_OverlayMutex;

//-----------------------------------------------------------------------------
// Purpose: Draws all overlays
//-----------------------------------------------------------------------------
// clang-format off
AUTOHOOK(DrawAllOverlays, engine.dll + 0xAB780,
void, __fastcall, (char a1))
// clang-format on
{
	// TODO [Fifty]: Check enable_debug_overlays here
	DrawAllOverlays(a1);

	s_OverlayMutex.lock();

	DrawEntTriggers();

	s_OverlayMutex.unlock();
}

ON_DLL_LOAD("server.dll", ShowTriggersServer, (CModule module))
{
	AUTOHOOK_DISPATCH()

	RegisterConCommand("ns_showtriggers", ConCommand_ns_showtriggers, "Toggles showing ent triggers", FCVAR_NONE);
}

ON_DLL_LOAD("engine.dll", ShowTriggersEngine, (CModule module))
{
	AUTOHOOK_DISPATCH()

	RenderLine = module.Offset(0x192A70).As<RenderLineType>();
	RenderBox = module.Offset(0x192520).As<RenderBoxType>();
	RenderWireframeBox = module.Offset(0x193DA0).As<RenderBoxType>();
	RenderWireframeSweptBox = module.Offset(0x1945A0).As<RenderWireframeSweptBoxType>();
	RenderTriangle = module.Offset(0x193940).As<RenderTriangleType>();
	RenderAxis = module.Offset(0x1924D0).As<RenderAxisType>();
	RenderSphere = module.Offset(0x194170).As<RenderSphereType>();
}

ON_DLL_LOAD("client.dll", ShowTriggersClient, (CModule module))
{
	AUTOHOOK_DISPATCH()
}
