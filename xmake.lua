add_rules("mode.debug", "mode.release")

set_languages("c89", "c++20")

target("immutable-cpp")
    set_kind("binary")
    add_includedirs("include")
    add_cxxflags("-fconcepts")
    if is_mode("release") then
        set_optimize("fastest")
    end
    add_files("src/*.cpp")
