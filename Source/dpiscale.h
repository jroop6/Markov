#pragma once

class DPIScale
{
	static FLOAT PIXELS_PER_LOGICAL_INCH;
	static float scaleX;
	static float scaleY;

public:
	static void Initialize(ID2D1Factory *pFactory)
	{
		FLOAT dpiX;
		FLOAT dpiY;
		pFactory->GetDesktopDpi(&dpiX, &dpiY);
		scaleX = dpiX / PIXELS_PER_LOGICAL_INCH;
		scaleY = dpiY / PIXELS_PER_LOGICAL_INCH;
	}

	template <typename T> static D2D1_POINT_2F PixelsToDips(T x, T y)
	{
		return D2D1::Point2F(static_cast<float>(x) / scaleX, static_cast<float>(y) / scaleX);
	}
	template <typename T> static D2D1_POINT_2F DipsToPixel(T x, T y)
	{
		return D2D1::Point2F(static_cast<float>(x) * scaleX, static_cast<float>(y) * scaleX);
	}

};

// Initializers:
float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;
FLOAT DPIScale::PIXELS_PER_LOGICAL_INCH = 96.0; // As defined by Microsoft.

