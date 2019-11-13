#! /bin/bash

count=1
numkeys=0
num=0

#declare -a arr=("esc" "f1" "f2" "f3" "f4" "f5" "f6" "f7" "f8" "f9" "f10" "f11" "f12" "prtscr" "del" "~" "1" "2" "3" "4" "5" "6" "7" "8" "9" "0" "-" "=" "backspace" "home" "tab" "q" "w" "e" "r" "t" "y" "u" "i" "o" "p" "[" "]" "\\" "pgup" "capslock" "a" "s" "d" "f" "g" "h" "j" "k" "l" ";" "'" "enter" "pgon" "lshift" "z" "x" "c" "v" "b" "n" "m" "," "." "fowardslash" "rshift" "up" "end" "lctrl" "win" "lalt" "space" "backslash" "ralt" "fn" "rctrl" "left" "down" "right")

declare -a arr=("esc" "f1" "f2" "f3" "f4" "f5" "f6" "f7" "f8" "f9" "f10" "f11" "f12" "prtscr" "scroll-lock" "insert" "delete"  "~" "1" "2" "3" "4" "5" "6" "7" "8" "9" "0" "-" "=" "backspace" "home" "tab" "q" "w" "e" "r" "t" "y" "u" "i" "o" "p" "[" "]" "\\" "capslock" "a" "s" "d" "f" "g" "h" "j" "k" "l" ";" "'" "enter" "lshift" "z" "x" "c" "v" "b" "n" "m" "," "." "fowardslash" "rshift" "up" "end" "lctrl" "win" "lalt" "space" "backslash" "ralt" "fn" "rctrl" "left" "down" "right")

mkdir -p "keycodes"

while [ $count -le 168 ]
do
    if ! (($count % 2 == 0)); then
    sed -n "$count p" "$1" > keycodes-${arr[$num]}.txt && mv keycodes-${arr[$num]}.txt keycodes
    num=$((num + 1))
    fi

    count=$((count + 1))
done
