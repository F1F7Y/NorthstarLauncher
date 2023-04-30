#include "showtriggers.h"
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


std::vector<Trigger_t> vecTriggers;
bool bHasBuiltMeshes = false;
bool bShouldDrawTriggers = false;

typedef void (*RenderLineType)(const Vector3& vecAbsStart, const Vector3& vecAbsEnd, int r, int g, int b, bool test, float duration);
static RenderLineType RenderLine;

AUTOHOOK_INIT()


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

bool FloatCompare(float A, float B)
{
	return fabsf(A - B) < 0.0001;
}

bool VertexCompare(Vertex_t& vertA, Vertex_t& vertB)
{
	return FloatCompare(vertA.x, vertB.x) && FloatCompare(vertA.y, vertB.y) && FloatCompare(vertA.z, vertB.z);
}

void BuildTriggerMeshes()
{
	bHasBuiltMeshes = true;

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
					const Plane_t& planeB = brush.vecPlanes.at(j);
					for (std::size_t k = 0; k < brush.vecPlanes.size(); k++)
					{
						const Plane_t& planeC = brush.vecPlanes.at(k);

						Vector3 s;
						s.x = planeB.b * planeC.c - planeB.c * planeC.b;
						s.y = planeB.c * planeC.a - planeB.a * planeC.c;
						s.z = planeB.a * planeC.b - planeB.b * planeC.a;

						float d = planeA.a * s.x + planeA.b * s.y + planeA.c * s.z;

						Vertex_t vertex;
						vertex.x = (-planeA.d * (planeB.b * planeC.c - planeB.c * planeC.b) - planeB.d * (planeC.b * planeA.c - planeC.c * planeA.b) - planeC.d * (planeA.b * planeB.c - planeA.c * planeB.b)) / d;
						vertex.y = (-planeA.d * (planeB.c * planeC.a - planeB.a * planeC.c) - planeB.d * (planeC.c * planeA.a - planeC.a * planeA.c) - planeC.d * (planeA.c * planeB.a - planeA.a * planeB.c)) / d;
						vertex.z = (-planeA.d * (planeB.a * planeC.b - planeB.b * planeC.a) - planeB.d * (planeC.a * planeA.b - planeC.b * planeA.a) - planeC.d * (planeA.a * planeB.b - planeA.b * planeB.a)) / d;

						if (vertex.x != vertex.x || vertex.y != vertex.y || vertex.z != vertex.z)
							continue;

						bool bExists = false;
						for (Vertex_t& vert : planeA.vecVertices)
						{
							bExists |= VertexCompare(vert, vertex);
						}

						if(!bExists)
							planeA.vecVertices.emplace_back(vertex);
					}
				}

			}
		}
	}
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
				for (std::size_t i = 1; i < plane.vecVertices.size(); i++)
				{
					const Vertex_t& vertexA = plane.vecVertices.at(i - 1);
					const Vertex_t& vertexB = plane.vecVertices.at(i);

					Vector3 vectorA = Vector3(vertexA.x, vertexA.y, vertexA.z);
					Vector3 vectorB = Vector3(vertexB.x, vertexB.y, vertexB.z);

					RenderLine(vectorA, vectorB, 255, 0, 0, true, 0.0);
				}
			}
		}
	}

	//Vector3 start(0.0, 0.0, 0.0);
	//RenderLine(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 255.0), 255, 0, 0, true, 5.0);
}

void ConCommand_ns_showtriggers(const CCommand& arg)
{
	if (!bHasBuiltMeshes)
		BuildTriggerMeshes();

	if (bShouldDrawTriggers)
		spdlog::info("disabled drawing ent triggers!");
	else
		spdlog::info("enabled drawing ent triggers!");


	//RenderLine(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 255.0), 255, 0, 0, true, 5.0);
	bShouldDrawTriggers = !bShouldDrawTriggers;
	//spdlog::info("Toggled bShouldDrawTriggers!");
	//spdlog::info("Num triggers: {}", vecTriggers.size());
	
	/*spdlog::info("Triggers:");
	for (const Trigger_t& trigger : vecTriggers)
	{
		spdlog::info("Trigger:");
		for (const Brush_t& brush : trigger.vecBrushes)
		{
			spdlog::info("Brush:");
			for (const Plane_t& plane : brush.vecPlanes)
			{
				spdlog::info("Plane {} {} {} {}", plane.a, plane.b, plane.c, plane.d );
				for (const Vertex_t& vertex : plane.vecVertices)
				{
					spdlog::info("Vertex {} {} {}", vertex.x, vertex.y, vertex.z);
				}
			}
		}
	}*/
}



ON_DLL_LOAD("server.dll", ShowTriggersServer, (CModule module))
{
	AUTOHOOK_DISPATCH()

	RegisterConCommand("ns_showtriggers", ConCommand_ns_showtriggers, "Toggles showing ent triggers", FCVAR_NONE);
}

ON_DLL_LOAD("engine.dll", ShowTriggersEngine, (CModule module))
{
	AUTOHOOK_DISPATCH()
}

ON_DLL_LOAD("client.dll", ShowTriggersClient, (CModule module))
{
	AUTOHOOK_DISPATCH()

	RenderLine = module.Offset(0x1CCF40).As<RenderLineType>();
}
