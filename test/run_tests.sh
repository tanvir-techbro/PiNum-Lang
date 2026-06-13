#!/bin/bash

#******************************************************************#
#   _____ _ _   _                       _                          #
#  |  __ (_) \ | |                     | |                         #
#  | |__) ||  \| |_   _ _ __ ___ ______| |     __ _ _ __   __ _    #
#  |  ___/ | . ` | | | | '_ ` _ \______| |    / _` | '_ \ / _` |   #
#  | |   | | |\  | |_| | | | | | |     | |___| (_| | | | | (_| |   #
#  |_|   | |_| \_|\__,_|_| |_| |_|     |______\__,_|_| |_|\__, |   #
#                                                          __/ |   #
#                                                         |___/    #
#                                                                  #
#  Copyright (c) 2026 tanvir-techbro.                              #
#  You may opt to use, copy, modify, merge, publish, distribute    #
#  and/or sell copies of the Software, and permit persons to whom  #
#  the Software is furnished to do so, under the conditions of the #
#  LICENSE.                                                        #
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, #
#  EXPRESS OR IMPLIED.                                             #
#                                                                  #
#  If you find any bug you would be highly encouraged to create a  #
#  github issue at <https://github.com/tanvir-techbro/PiNum-Lang>  #
#  or contact <surjointelligence.team@gmail.com>                   #
#******************************************************************#

# NOTE: remember to uncomment the debugger functions before running the tests, so you can verify all the things are running correct.
# This script uses pinum from bin/pinum so before running this script make sure to run "make".
# required dependencies: valgrind

echo "Running lexer tests with valgrind..."
echo ""
echo "Lexer test 1"
valgrind --leak-check=full --show-leak-kinds=all bin/pinum test/lexer-tests/standard_test-1.pn
echo ""
echo "Lexer test 2"
valgrind --leak-check=full --show-leak-kinds=all bin/pinum test/lexer-tests/standard_test-2.pn
echo ""
echo "Lexer test 3"
valgrind --leak-check=full --show-leak-kinds=all bin/pinum test/lexer-tests/standard_test-3.pn
echo ""
echo "Lexer tests done, read the valgrind reports and check the debugger result to verify the output."
