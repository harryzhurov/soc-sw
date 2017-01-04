#-------------------------------------------------------------------------------
define rst
    printf "*******************\n"
    printf "Reset\n"
    monitor reset halt
end
#-------------------------------------------------------------------------------
define cn
    target remote :3333
end
#-------------------------------------------------------------------------------
define db
    if $argc == 1
        printf "*******************\n"
        printf "Delete breakpoint:\n"
        info breakpoints $arg0
        printf "*******************\n"
        delete breakpoints $arg0
    else 
        printf "Delete all breakpoints\n"
        delete breakpoints
    end

    printf "\n*******************\n"
    printf "Current breakpoints list:\n"
    info breakpoints
    printf "*******************\n"
end
document db
Usage: db [n]
Delete breakpoint specified by number or all breakpoints if no arg
end
#-------------------------------------------------------------------------------
define go_main
    printf "Load program\n"
    break main
    continue
    clear main
end    
#-------------------------------------------------------------------------------
define rl
    load
	rst
    go_main
end
#-------------------------------------------------------------------------------

