egrep -rl ' $' --include *.c *  | xargs sed -i 's/\s\+$//g'
egrep -rl ' $' --include *.h *  | xargs sed -i 's/\s\+$//g'
egrep -rl ' $' --include Makefile *  | xargs sed -i 's/\s\+$//g'
