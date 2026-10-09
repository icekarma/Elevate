#include <iostream>
#include <span>
#include <string>
#include <string_view>

#pragma warning( push )
#pragma warning( disable: 4365 ) // C4365: 'argument': conversion from '<type_1>' to '<type_2>', signed/unsigned mismatch
#pragma warning( disable: 4668 ) // C4668: '<macro name>' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#pragma warning( disable: 4865 ) // C4865: the underlying type of '<type name>' will change when '/Zc:enumTypes' is specified

#include <winsdkver.h>
#include <sdkddkver.h>
#include <windows.h>

#pragma warning( pop )

int wmain( int argc, wchar_t* argv[] ) {
    if ( argc < 2 ) {
        std::wcerr << L"Usage: elevate <executable> [args...]\n";
        return 1;
    }

    std::wstring parameters;
    if ( argc > 2 ) {
        for ( auto const arg_ : std::span<wchar_t*> { argv + 2, static_cast<size_t>( argc - 2 ) } ) {
            if ( std::wstring_view const arg { arg_ }; arg.find_first_of( L" \t" ) != std::wstring_view::npos ) {
                parameters += L'"';
                parameters += arg;
                parameters += L'"';
            } else {
                parameters += arg;
            }
            parameters += L' ';
        }
        if ( !parameters.empty( ) ) {
            parameters.pop_back( );
        }
    }

    SHELLEXECUTEINFOW sei { sizeof( sei ) };
    sei.lpVerb       = L"runas";
    sei.lpFile       = argv[1];
    sei.lpParameters = parameters.empty( ) ? nullptr : parameters.c_str( );
    sei.nShow        = SW_SHOWNORMAL;
    sei.fMask        = SEE_MASK_DEFAULT | SEE_MASK_FLAG_LOG_USAGE;

    SetLastError( 0 );
    if ( !ShellExecuteExW( &sei ) ) {
        if ( DWORD err { GetLastError( ) }; err == ERROR_CANCELLED ) {
            std::wcerr << L"Elevation prompt was declined by the user.\n";
            return 2;
        } else {
            std::wcerr << L"Failed to execute process. Error code: " << err << L"\n";
            return 1;
        }
    }

    return 0;
}
