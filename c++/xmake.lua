set_languages("cxx17")
set_rules("mode.debug", "mode.release")
add_includedirs("/usr/local/include")
add_linkdirs("/usr/local/lib")
add_links("boost_filesystem","boost_system", "leveldb")

if is_mode("debug") then
    set_options("-g -Wall")
    set_optimize("none")
end

-- define target
target("tst-skyline")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-skyline.cc")

-- define target
target("tst-leveldb_open")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-leveldb_open.cc")

-- define target
target("tst-int64_to_string")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-int64_to_string.cc")

-- define target
target("tst-gen_uuid")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-gen_uuid.cc")

-- define target
target("tst_c++17_count_tmp")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst_c++17_count_tmp.cc")

-- define target
target("tst-string_find_first_of")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-string_find_first_of.cc")

-- define target
target("tst-string_rfind")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-string_rfind.cc")

-- define target
target("tst-cstdio_rename")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-cstdio_rename.cc")

-- define target
target("enum")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("enum.cc")

-- define target
target("tst-popen")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-popen.cc")

-- define target
target("tst_file_locker")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst_file_locker.cc")

-- define target
target("tst-split")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-split.cc")

-- define target
target("tst-boost_filesystem")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-boost_filesystem.cc")

-- define target
target("tst-move")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-move.cc")

-- define target
target("tst-function")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-function.cc")

-- define target
target("tst-async_log")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-async_log.cc")

-- define target
target("tst-boost_dir_sst_size")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-boost_dir_sst_size.cc")

-- define target
target("tst-shared_ptr")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-shared_ptr.cc")

-- define target
target("tst-linux_ftw")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-linux_ftw.cc")

-- define target
target("tst-multi_thread_map")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-multi_thread_map.cc")

-- define target
target("tst-buf_stream")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-buf_stream.cc")

-- define target
target("ofstream")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("ofstream.cc")

-- define target
target("tst")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst.cc")

-- define target
target("tst-cv")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-cv.cc")

-- define target
target("tst-sprintf")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-sprintf.cc")

-- define target
target("tst-boost_remove_all")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-boost_remove_all.cc")

-- define target
target("tst-bits")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-bits.cc")

-- define target
target("tst-chrono")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-chrono.cc")

-- define target
target("tst-string_pointer")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-string_pointer.cc")

-- define target
target("tst-sstream")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-sstream.cc")

-- define target
target("tst-c++0x")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-c++0x.cc")

-- define target
target("tst_string_stream")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst_string_stream.cc")

-- define target
target("tst_set_vector_map")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst_set_vector_map.cc")

-- define target
target("tst_string_substr")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst_string_substr.cc")

-- define target
target("tst-string")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-string.cc")

-- define target
target("tst-return_private_ref")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-return_private_ref.cc")

-- define target
target("tst-unordered_map")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-unordered_map.cc")

-- define target
target("tst-print_time")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-print_time.cc")

-- define target
target("tst-mem_fun")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-mem_fun.cc")

-- define target
target("tst-linux_flock")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-linux_flock.cc")

-- define target
target("tst-pointer")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-pointer.cc")

-- define target
target("tst-getline")

    -- set kind
    set_kind("binary")

    -- add files
    add_files("tst-getline.cc")

--
-- FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "cxx11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox", "z", "pthread")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--
-- 7. If you want to known more usage about xmake, please see http://xmake.io/#/home
--

