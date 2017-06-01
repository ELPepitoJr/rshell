#!/bin/sh
make

echo "rbrbrb "hi everyone" || (echo test && edhhdhdh barb || echo UCR)" > text.txt
bin/rshell text.txt

echo "rbrbrb "hi everyone" || (echo test || edhhdhdh barb || echo UCR) && echo no" > text.txt
bin/rshell text.txt

echo "rbrbrb "hi everyone"; (echo test || edhhdhdh barb || echo UCR) || (echo hi && echo way" > text.txt
bin/rshell text.txt

echo "rbrbrb "hi everyone"; (merp test || edhhdhdh barb || fail UCR) || (echo "hi everyone" && echo wayyyy)" > text.txt
bin/rshell text.txt

echo "echo "hi everyone" || (merp test || edhhdhdh barb || fail UCR) || (echo "lovely day" && echo wayyyy)" > text.txt
bin/rshell text.txt

echo "echo hi; (merp test || (echo no; echo way) || fail UCR) && (echo "hi everyone" && echo wayyyy)" > text.txt
bin/rshell text.txt

echo "echo hi; ((merp test || echo no) || echo way) || fail UCR) && (echo "hi everyone" && echo wayyyy)" > text.txt
bin/rshell text.txt

echo "echo hi; ((merp test; echo no) || echo way) || fail UCR) || (echo "hi everyone" && echo wayyyy)" > text.txt
bin/rshell text.txt