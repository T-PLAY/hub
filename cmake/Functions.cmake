



function(custom_target_added name)
#    get_property(old_count GLOBAL PROPERTY N_DEPENDENCY)
#    math( EXPR new_count "${old_count}+1" )
#    set_property(GLOBAL PROPERTY N_DEPENDENCY "${new_count}")

    get_property(old_list GLOBAL PROPERTY CUSTOM_TARGETS_FOUNDED)
#    if (${old_count} STREQUAL "0")
#        set(new_list "${name}")
#    else()
        set(new_list "${old_list} ${name}")
#    endif()
    set_property(GLOBAL PROPERTY CUSTOM_TARGETS_FOUNDED "${new_list}")
endfunction()
