/* Linker version script: hide every spdlog symbol from the dynamic
 * symbol table of the .so / executable using this script.
 *
 * NRPGeneralLibrary (and every consumer .so that transitively links
 * libspdlog.a) bakes spdlog 1.11.0 / fmt 9 in statically. On focal,
 * ROS 2 Foxy's librcl_logging_spdlog.so is built against the system
 * libspdlog.so.1 (1.5.0 / fmt 6). When rclcpp::init() dlopens the
 * shim, the dynamic loader resolves the shim's undefined spdlog
 * VTABLE and typeinfo references against whichever .so is already
 * loaded — and NRPGeneralLibrary's spdlog 1.11 vtable wins the
 * lookup over the system 1.5 .so. The spdlog 1.5 set_formatter call
 * then constructs a pattern_formatter using the 1.11 vtable, whose
 * destructor reads a custom_flag_formatter map at the wrong layout
 * offset, and the process SIGSEGVs in __memset_avx512_erms.
 *
 * Patterns match against the MANGLED Itanium-C++ symbol names rather
 * than demangled, because `vtable for spdlog::pattern_formatter`
 * (demangled) and `typeinfo for spdlog::*` don't begin with `spdlog`
 * so a naive demangled `spdlog::*` glob misses them. The mangled
 * forms are:
 *
 *   _ZN6spdlog...   regular spdlog::* symbols (functions, members)
 *   _ZTV*spdlog*    vtable for spdlog::*
 *   _ZTI*spdlog*    typeinfo for spdlog::*
 *   _ZTS*spdlog*    typeinfo name for spdlog::*
 *   _ZNK6spdlog...  const member functions
 *   _ZNSt..._Hashtable<...spdlog...   instantiations of std types
 *                   parametrised by spdlog types (e.g. the
 *                   custom_flag_formatter unordered_map)
 *
 * Same set for fmt::v9::* — namespace is `_ZN3fmt2v9` mangled.
 */
{
    local:
        _ZN6spdlog*;
        _ZNK6spdlog*;
        _ZTV*6spdlog*;
        _ZTI*6spdlog*;
        _ZTS*6spdlog*;
        _ZN3fmt2v9*;
        _ZNK3fmt2v9*;
        _ZTV*3fmt2v9*;
        _ZTI*3fmt2v9*;
        _ZTS*3fmt2v9*;
        _ZN*_Hashtable*spdlog*;
        _ZNK*_Hashtable*spdlog*;
        _ZN*unique_ptr*spdlog*;
};
