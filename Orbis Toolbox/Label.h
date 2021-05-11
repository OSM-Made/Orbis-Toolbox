#pragma once

class Label : public Widget
{
public:
	enum VerticalAlignment
	{
		vTop,
		vBottom,
		vCenter
	};

	enum HorizontalAlignment
	{
		hLeft,
		hCenter,
		hRight
	};

	enum FontStyle
	{
		fsNormal,
		fsItalic = 2U
	};

	enum FontWeight
	{
		fwNormal, //Obsolete: This value is not used for Orbis. Use 'Light' or 'Medium'.
		fwBold, //Obsolete: Use 'Medium' instead of Bold.
		fwLight,
		fwMedium,
		fwLegacyBold = 1000U //Obsolete: This value is for GLS. Use 'Medium' instead of LegacyBold.
	};

	void Set_Location(float X, float Y);
	void Set_Text(const char* Text);
	void Set_Font(int Size, FontStyle Style, FontWeight Weight);
	void Set_Alignment(VerticalAlignment Vertical_Align, HorizontalAlignment Horizontal_Align);
	void Set_Colour(float R, float G, float B, float A);

	float Get_Text_Width();
	float Get_Text_Height();

	Label(const char* Name);
	Label(const char* Name, float X, float Y, const char* Text, int Size, FontStyle Style, FontWeight Weight, VerticalAlignment Vertical_Align, HorizontalAlignment Horizontal_Align, float R, float G, float B, float A);
	~Label();

private:
	MonoClass* Label_Class;
};