# OpenMMORPG

Developed with Unreal Engine 4


## Dependencies


### GRPC
This project uses grpc v1.32.0 for its server communications. While it works fine, building it for use with unreal might be a little tricky, so I'm providing code snippet how to point GRPC building process to Unreal libs.
The reason for doing following action - to make sure you can package your project later, because if not building against UE's deps you'll get linking errors.

```
SET UE_ROOT=Path_to_unreal_engine_folder 

cmake .. -G "Visual Studio 16 2019" -A x64 ^
    -DCMAKE_CXX_STANDARD_LIBRARIES="Crypt32.Lib User32.lib Advapi32.lib" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_CONFIGURATION_TYPES=Release ^
    -Dprotobuf_BUILD_TESTS=OFF ^
    -DgRPC_ZLIB_PROVIDER=package ^
    -DZLIB_INCLUDE_DIR="%UE_ROOT%\Engine\Source\ThirdParty\zlib\v1.2.8\include\Win64\VS2015" ^
    -DZLIB_LIBRARY_DEBUG="%UE_ROOT%\Engine\Source\ThirdParty\zlib\v1.2.8\lib\Win64\VS2015\Debug\zlibstatic.lib" ^
    -DZLIB_LIBRARY_RELEASE="%UE_ROOT%\Engine\Source\ThirdParty\zlib\v1.2.8\lib\Win64\VS2015\Release\zlibstatic.lib" ^
    -DgRPC_SSL_PROVIDER=package ^
    -DLIB_EAY_LIBRARY_DEBUG="%UE_ROOT%\Engine\Source\ThirdParty\OpenSSL\1.1.1\Lib\Win64\VS2015\Debug\libcrypto.lib" ^
    -DLIB_EAY_LIBRARY_RELEASE="%UE_ROOT%\Engine\Source\ThirdParty\OpenSSL\1.1.1\Lib\Win64\VS2015\Release\libcrypto.lib" ^
    -DLIB_EAY_DEBUG="%UE_ROOT%\Engine\Source\ThirdParty\OpenSSL\1.1.1\Lib\Win64\VS2015\Debug\libcrypto.lib" ^
    -DLIB_EAY_RELEASE="%UE_ROOT%\Engine\Source\ThirdParty\OpenSSL\1.1.1\Lib\Win64\VS2015\Release\libcrypto.lib" ^
    -DOPENSSL_INCLUDE_DIR="%UE_ROOT%\Engine\Source\ThirdParty\OpenSSL\1.1.1\include\Win64\VS2015" ^
    -DSSL_EAY_DEBUG="%UE_ROOT%\Engine\Source\ThirdParty\OpenSSL\1.1.1\Lib\Win64\VS2015\Debug\libssl.lib" ^
    -DSSL_EAY_LIBRARY_DEBUG="%UE_ROOT%\Engine\Source\ThirdParty\OpenSSL\1.1.1\Lib\Win64\VS2015\Debug\libssl.lib" ^
    -DSSL_EAY_LIBRARY_RELEASE="%UE_ROOT%\Engine\Source\ThirdParty\OpenSSL\1.1.1\Lib\Win64\VS2015\Release\libssl.lib" ^
    -DSSL_EAY_RELEASE="%UE_ROOT%\Engine\Source\ThirdParty\OpenSSL\1.1.1\Lib\Win64\VS2015\Release\libssl.lib"
    
cmake --build . --target ALL_BUILD --config Release
```

After compiling stuff, you also need to modify build.cs script adding following deps

```
AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenSSL");
AddEngineThirdPartyPrivateStaticDependencies(Target, "zlib");
```
