#pragma once

struct FVector2D {
	float x = 0.0f;
	float y = 0.0f;

	FVector2D(float inX, float inY) : x(inX), y(inY) {}
	FVector2D() {}

	FVector2D operator*(float scalar) const {
		return FVector2D(x * scalar, y * scalar);
	}

	FVector2D operator-(FVector2D v) const {
		return FVector2D(x - v.x, y - v.y);
	}

	FVector2D operator-=(FVector2D v) const {
		return FVector2D(x - v.x, y - v.y);
	}

	FVector2D operator+(FVector2D v) const {
		return FVector2D(x + v.x, y + v.y);
	}

	FVector2D operator+=(FVector2D v) const {
		return FVector2D(x + v.x, y + v.y);
	}

	FVector2D operator*(FVector2D v) const {
		return FVector2D(x * v.x, y * v.y);
	}

	FVector2D operator*=(FVector2D v) const {
		return FVector2D(x * v.x, y * v.y);
	}

	bool operator==(FVector2D v) const {
		return x  == v.x && y == v.y;
	}

	bool operator!=(FVector2D v) const {
		return x != v.x || y != v.y;
	}

	FVector2D Inverse() const
	{
		return FVector2D{ x * -1, y * -1 };
	}

	FVector2D Ortogonal() const
	{
		return FVector2D{ -y, x };
	}

	static float DotProduct(const FVector2D& A, const FVector2D& B)
	{
		return A.x * B.x + A.y * B.y;
	}
};

struct FTransform {
	FVector2D location = FVector2D{0.0f, 0.0f};
	float rotation = 0.0f;
	float scale = 1.0f;

};