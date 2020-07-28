#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

const char *(WINAPI *wglGetExtensionsStringARB)(HDC hDC)=NULL;

BOOL (WINAPI *wglGetPixelFormatAttribivARB)(HDC hDC, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues)=NULL;

#define WGL_NUMBER_PIXEL_FORMATS_ARB					0x2000
#define WGL_DRAW_TO_WINDOW_ARB							0x2001
#define WGL_DRAW_TO_BITMAP_ARB							0x2002
#define WGL_ACCELERATION_ARB							0x2003
#define WGL_NEED_PALETTE_ARB							0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB						0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB						0x2006
#define WGL_SWAP_METHOD_ARB								0x2007
#define WGL_NUMBER_OVERLAYS_ARB							0x2008
#define WGL_NUMBER_UNDERLAYS_ARB						0x2009
#define WGL_TRANSPARENT_ARB								0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB					0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB					0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB					0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB					0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB					0x203B
#define WGL_SHARE_DEPTH_ARB								0x200C
#define WGL_SHARE_STENCIL_ARB							0x200D
#define WGL_SHARE_ACCUM_ARB								0x200E
#define WGL_SUPPORT_GDI_ARB								0x200F
#define WGL_SUPPORT_OPENGL_ARB							0x2010
#define WGL_DOUBLE_BUFFER_ARB							0x2011
#define WGL_STEREO_ARB									0x2012
#define WGL_PIXEL_TYPE_ARB								0x2013
#define WGL_COLOR_BITS_ARB								0x2014
#define WGL_RED_BITS_ARB								0x2015
#define WGL_RED_SHIFT_ARB								0x2016
#define WGL_GREEN_BITS_ARB								0x2017
#define WGL_GREEN_SHIFT_ARB								0x2018
#define WGL_BLUE_BITS_ARB								0x2019
#define WGL_BLUE_SHIFT_ARB								0x201A
#define WGL_ALPHA_BITS_ARB								0x201B
#define WGL_ALPHA_SHIFT_ARB								0x201C
#define WGL_ACCUM_BITS_ARB								0x201D
#define WGL_ACCUM_RED_BITS_ARB							0x201E
#define WGL_ACCUM_GREEN_BITS_ARB						0x201F
#define WGL_ACCUM_BLUE_BITS_ARB							0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB						0x2021
#define WGL_DEPTH_BITS_ARB								0x2022
#define WGL_STENCIL_BITS_ARB							0x2023
#define WGL_AUX_BUFFERS_ARB								0x2024
#define WGL_NO_ACCELERATION_ARB							0x2025
#define WGL_GENERIC_ACCELERATION_ARB					0x2026
#define WGL_FULL_ACCELERATION_ARB						0x2027
#define WGL_SWAP_EXCHANGE_ARB							0x2028
#define WGL_SWAP_COPY_ARB								0x2029
#define WGL_SWAP_UNDEFINED_ARB							0x202A
#define WGL_TYPE_RGBA_ARB								0x202B
#define WGL_TYPE_COLORINDEX_ARB							0x202C

#define WGL_SAMPLE_BUFFERS_ARB							0x2041
#define WGL_SAMPLES_ARB									0x2042

#define WGL_DRAW_TO_PBUFFER_ARB							0x202D
#define WGL_MAX_PBUFFER_WIDTH_ARB						0x202F
#define WGL_MAX_PBUFFER_HEIGHT_ARB						0x2030

#define WGL_TYPE_RGBA_FLOAT_ATI							0x21A0

#define WGL_FLOAT_COMPONENTS_NV							0x20B0

#define GL_SHADING_LANGUAGE_VERSION_ARB					0x8B8C

int main(void)
{
	PIXELFORMATDESCRIPTOR pfd;
	int PixelFormat;
	int Attrib[128], Result[128], i;
	const char *GLextensions=NULL, *WGLextensions=NULL;
	FILE *stream=NULL;

	hWnd=CreateWindow("PixelFormat", "PixelFormat", WS_OVERLAPPEDWINDOW, 0, 0, 1, 0, NULL, NULL, GetModuleHandle(NULL), NULL);

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion=1;
	pfd.dwFlags=PFD_SUPPORT_OPENGL;
	pfd.iPixelType=PFD_TYPE_RGBA;
	pfd.cColorBits=16;
	pfd.cDepthBits=16;

	hDC=GetDC(hWnd);

	if(!(PixelFormat=ChoosePixelFormat(hDC, &pfd)))
	{
		fprintf(stderr, "ChoosePixelFormat failed.\n");
		return -1;
	}

	if(!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		fprintf(stderr, "SetPixelFormat failed.\n");
		return -1;
	}

	if(!(hRC=wglCreateContext(hDC)))
	{
		fprintf(stderr, "wglCreateContext failed.\n");
		return -1;
	}

	wglMakeCurrent(hDC, hRC);

	wglGetPixelFormatAttribivARB=(void *)wglGetProcAddress("wglGetPixelFormatAttribivARB");
	wglGetExtensionsStringARB=(void *)wglGetProcAddress("wglGetExtensionsStringARB");

	GLextensions=glGetString(GL_EXTENSIONS);

	if(wglGetExtensionsStringARB!=NULL)
		WGLextensions=wglGetExtensionsStringARB(hDC);

	if(wglGetPixelFormatAttribivARB==NULL)
	{
		fprintf(stderr, "wglGetProcAddress failed.\n");

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
		DeleteDC(hDC);
		DestroyWindow(hWnd);

		return -1;
	}

	Attrib[0]=WGL_NUMBER_PIXEL_FORMATS_ARB;
	Attrib[1]=0;

	wglGetPixelFormatAttribivARB(hDC, 1, 0, 2, Attrib, Result);

	PixelFormat=Result[0];

	Attrib[ 0]=WGL_DRAW_TO_WINDOW_ARB;
	Attrib[ 1]=WGL_DRAW_TO_BITMAP_ARB;
	Attrib[ 2]=WGL_ACCELERATION_ARB;
	Attrib[ 3]=WGL_NEED_PALETTE_ARB;
	Attrib[ 4]=WGL_NEED_SYSTEM_PALETTE_ARB;
	Attrib[ 5]=WGL_SWAP_LAYER_BUFFERS_ARB;
	Attrib[ 6]=WGL_SWAP_METHOD_ARB;
	Attrib[ 7]=WGL_NUMBER_OVERLAYS_ARB;
	Attrib[ 8]=WGL_NUMBER_UNDERLAYS_ARB;
	Attrib[ 9]=WGL_SUPPORT_GDI_ARB;
	Attrib[10]=WGL_SUPPORT_OPENGL_ARB;
	Attrib[11]=WGL_DOUBLE_BUFFER_ARB;
	Attrib[12]=WGL_PIXEL_TYPE_ARB;
	Attrib[13]=WGL_COLOR_BITS_ARB;
	Attrib[14]=WGL_RED_BITS_ARB;
	Attrib[15]=WGL_GREEN_BITS_ARB;
	Attrib[16]=WGL_BLUE_BITS_ARB;
	Attrib[17]=WGL_ALPHA_BITS_ARB;
	Attrib[18]=WGL_ACCUM_BITS_ARB;
	Attrib[19]=WGL_ACCUM_RED_BITS_ARB;
	Attrib[20]=WGL_ACCUM_GREEN_BITS_ARB;
	Attrib[21]=WGL_ACCUM_BLUE_BITS_ARB;
	Attrib[22]=WGL_ACCUM_ALPHA_BITS_ARB;
	Attrib[23]=WGL_DEPTH_BITS_ARB;
	Attrib[24]=WGL_STENCIL_BITS_ARB;
	Attrib[25]=WGL_AUX_BUFFERS_ARB;
	Attrib[26]=WGL_SAMPLE_BUFFERS_ARB;
	Attrib[27]=WGL_SAMPLES_ARB;
	Attrib[28]=WGL_DRAW_TO_PBUFFER_ARB;
	Attrib[29]=WGL_MAX_PBUFFER_WIDTH_ARB;
	Attrib[30]=WGL_MAX_PBUFFER_HEIGHT_ARB;
	Attrib[31]=WGL_FLOAT_COMPONENTS_NV;
	Attrib[32]=0;

	if((stream=fopen("results.html", "wt"))==NULL)
		return -1;

	fprintf(stream, "<html>\n"
					"<body>\n");
	fprintf(stream, "<p>%s %s<br>\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
	fprintf(stream, "Version: %s<br>\n", glGetString(GL_VERSION));

	if(strstr(GLextensions, "GL_ARB_shading_language_100"))
		fprintf(stream, "GLSL Ver: %s</p>\n", glGetString(GL_SHADING_LANGUAGE_VERSION_ARB));

	fprintf(stream, "<p>GL Extensions:<br>\n%s</p>\n", extensions);

	if(WGLextensions!=NULL)
		fprintf(stream, "<p>WGL Extensions:<br>\n%s</p>\n", extensions);

	fprintf(stream, "<table border=\"1\">\n"
					"<tr><th>#</th><th>Draw Window</th><th>Draw Bitmap</th><th>Acceleration</th><th>Palette</th><th>System Palette</th><th>Swap Layers</th><th>Swap Method</th><th>Overlays</th><th>Underlays</th><th>GDI</th><th>OpenGL</th><th>Double Buffer</th><th>Pixel Type</th><th>Color</th><th>R</th><th>G</th><th>B</th><th>A</th><th>Accum</th><th>R</th><th>G</th><th>B</th><th>A</th><th>Depth</th><th>Stencil</th><th>Aux</th><th>AA</th><th>AA Samples</th><th>Draw PBuffer</th><th>Width</th><th>Height</th></tr>\n");

	for(i=1;i<PixelFormat+1;i++)
	{
		wglGetPixelFormatAttribivARB(hDC, i, 0, 32, Attrib, Result);

		fprintf(stream, "<tr>");
		fprintf(stream, "<td>%d</td>", i);

		fprintf(stream, "<td>%s</td>", Result[ 0]?"<font color=\"#00FF00\">True</font>":"<font color=\"#FF0000\">False</font>");
		fprintf(stream, "<td>%s</td>", Result[ 1]?"<font color=\"#00FF00\">True</font>":"<font color=\"#FF0000\">False</font>");

		switch(Result[2])
		{
			case WGL_NO_ACCELERATION_ARB:
				fprintf(stream, "<td><font color=\"#FF0000\">None</font></td>");
				break;

			case WGL_GENERIC_ACCELERATION_ARB:
				fprintf(stream, "<td><font color=\"#FFFF00\">Generic</font></td>");
				break;

			case WGL_FULL_ACCELERATION_ARB:
				fprintf(stream, "<td><font color=\"#00FF00\">Full</font></td>");
				break;

			default:
				fprintf(stream, "<td>0x%4.4X</td>", Result[ 2]);
				break;
		}

		fprintf(stream, "<td>%s</td>", Result[ 3]?"<font color=\"#00FF00\">True</font>":"<font color=\"#FF0000\">False</font>");
		fprintf(stream, "<td>%s</td>", Result[ 4]?"<font color=\"#00FF00\">True</font>":"<font color=\"#FF0000\">False</font>");
		fprintf(stream, "<td>%s</td>", Result[ 5]?"<font color=\"#00FF00\">True</font>":"<font color=\"#FF0000\">False</font>");

		switch(Result[6])
		{
			case WGL_SWAP_EXCHANGE_ARB:
				fprintf(stream, "<td>Exchange</td>");
				break;

			case WGL_SWAP_COPY_ARB:
				fprintf(stream, "<td>Copy</td>");
				break;

			case WGL_SWAP_UNDEFINED_ARB:
				fprintf(stream, "<td>Undefined</td>");
				break;

			default:
				fprintf(stream, "<td>%4.4X</td>", Result[ 6]);
				break;
		}

		fprintf(stream, "<td>%d</td>", Result[ 7]);
		fprintf(stream, "<td>%d</td>", Result[ 8]);
		fprintf(stream, "<td>%s</td>", Result[ 9]?"<font color=\"#00FF00\">True</font>":"<font color=\"#FF0000\">False</font>");
		fprintf(stream, "<td>%s</td>", Result[10]?"<font color=\"#00FF00\">True</font>":"<font color=\"#FF0000\">False</font>");
		fprintf(stream, "<td>%s</td>", Result[11]?"<font color=\"#00FF00\">True</font>":"<font color=\"#FF0000\">False</font>");

		switch(Result[12])
		{
			case WGL_TYPE_RGBA_ARB:
				if(Result[31])
					fprintf(stream, "<td>RGBA Float</td>");
				else
					fprintf(stream, "<td>RGBA</td>");
				break;

			case WGL_TYPE_COLORINDEX_ARB:
				fprintf(stream, "<td>Color Index</td>");
				break;

			case WGL_TYPE_RGBA_FLOAT_ATI:
				fprintf(stream, "<td>RGBA Float</td>");
				break;

			default:
				fprintf(stream, "<td>%4.4X</td>", Result[12]);
				break;
		}

		fprintf(stream, "<td>%d</td>", Result[13]);
		fprintf(stream, "<td>%d</td>", Result[14]);
		fprintf(stream, "<td>%d</td>", Result[15]);
		fprintf(stream, "<td>%d</td>", Result[16]);
		fprintf(stream, "<td>%d</td>", Result[17]);
		fprintf(stream, "<td>%d</td>", Result[18]);
		fprintf(stream, "<td>%d</td>", Result[19]);
		fprintf(stream, "<td>%d</td>", Result[20]);
		fprintf(stream, "<td>%d</td>", Result[21]);
		fprintf(stream, "<td>%d</td>", Result[22]);
		fprintf(stream, "<td>%d</td>", Result[23]);
		fprintf(stream, "<td>%d</td>", Result[24]);
		fprintf(stream, "<td>%d</td>", Result[25]);
		fprintf(stream, "<td>%s</td>", Result[26]?"<font color=\"#00FF00\">True</font>":"<font color=\"#FF0000\">False</font>");
		fprintf(stream, "<td>%d</td>", Result[27]);
		fprintf(stream, "<td>%s</td>", Result[28]?"<font color=\"#00FF00\">True</font>":"<font color=\"#FF0000\">False</font>");
		fprintf(stream, "<td>%d</td>", Result[29]);
		fprintf(stream, "<td>%d</td>",Result[30]);

		fprintf(stream, "</tr>\n");
	}

	fprintf(stream, "</table>\n");
	fprintf(stream, "</body>\n");
	fprintf(stream, "</html>\n");

	fclose(stream);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	DeleteDC(hDC);
	DestroyWindow(hWnd);

	return 0;
}