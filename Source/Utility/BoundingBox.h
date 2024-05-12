#pragma once

struct FMeshBoundBox
{
	float width = 0.0f;
	float height = 0.0f;
	float x = 0.0f;
	float y = 0.0f;

	static bool GetOverlapX(const FMeshBoundBox& A, const FMeshBoundBox& B)
	{
		return (A.x < B.x + B.width) && (B.x < A.x + A.width);
	}

	static bool GetOverlapY(const FMeshBoundBox& A, const FMeshBoundBox& B)
	{
		return (A.y < B.y + B.height) && (B.y < A.y + A.height);
	}
};
