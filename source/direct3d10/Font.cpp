/*
* Copyright (c) 2007 SlimDX Group
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include <d3d10.h>
#include <d3dx10.h>

#include "GraphicsException.h"

#include "Device.h"
#include "Font.h"
#include "Sprite.h"

#include <vcclr.h>

namespace SlimDX
{
namespace Direct3D10
{
	Font::Font( IntPtr pointer )
	{
		if( pointer == IntPtr::Zero )
			throw gcnew ArgumentNullException( "pointer" );

		void* result;
		IUnknown* unknown = static_cast<IUnknown*>( pointer.ToPointer() );
		HRESULT hr = unknown->QueryInterface( IID_ID3DX10Font, &result );
		if( FAILED( hr ) )
			throw gcnew InvalidCastException( "Failed to QueryInterface on user-supplied pointer." );

		m_Pointer = static_cast<ID3DX10Font*>( result );
	}

	Font::Font( ID3DX10Font* pointer )
	{
		if( pointer == 0 )
			throw gcnew ArgumentNullException( "pointer" );

		m_Pointer = pointer;
	}
	
	Font::Font( Device^ device, int height, int width, int weight, int mipLevels, bool isItalic,
		int characterSet, int outputPrecision, int quality, int pitchAndFamily, String^ faceName )
	{
		pin_ptr<const wchar_t> pinned_name = PtrToStringChars( faceName );
		
		ID3DX10Font* font = 0;
		HRESULT hr = D3DX10CreateFont( device->DevicePointer, height, width, weight, mipLevels, isItalic,
			characterSet, outputPrecision, quality, pitchAndFamily, pinned_name, &font );
		GraphicsException::CheckHResult( hr );

		m_Pointer = font;
	}

	int Font::Draw( Sprite^ sprite, String^ text, Drawing::Rectangle rect, FontFlags flags, int color )
	{
		pin_ptr<const wchar_t> pinned_text = PtrToStringChars( text );
		RECT nativeRect = { rect.Left, rect.Top, rect.Right, rect.Bottom };
		int result = m_Pointer->DrawText( sprite->InternalPointer, pinned_text, text->Length, &nativeRect,
			static_cast<DWORD>( flags ), color );
		
		return result;
	}

	void Font::PreloadCharacters( int first, int last )
	{
		HRESULT hr = m_Pointer->PreloadCharacters( first, last );
		GraphicsException::CheckHResult( hr );
	}

	void Font::PreloadGlyphs( int first, int last )
	{
		HRESULT hr = m_Pointer->PreloadGlyphs( first, last );
		GraphicsException::CheckHResult( hr );
	}

	void Font::PreloadText( String^ text )
	{
		pin_ptr<const wchar_t> pinned_text = PtrToStringChars( text );
		HRESULT hr = m_Pointer->PreloadText( pinned_text, text->Length );
		GraphicsException::CheckHResult( hr );
	}
}
}