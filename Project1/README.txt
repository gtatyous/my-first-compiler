Name: Yousef Gtat
Summary: Using Flex for lexical analysis to create tokens to be parsed in the next project. This tokens defines sets of rules tubelar language uses. 
         The code works and all tests were passed localy and on Travis
         tokens.hpp: file defines a set of macros for internal implementations
         Extra error: handling were included in my regex rules. For example, 'cc' will result in Multichar error and 'c will result in unterminated char
         Extra credits: I have implemented the extra credits exercises, please test them for confirmation
         I could have printed the results of each regex rule on the spot, however, the way I used macros gives me more flexibility for any extra parsing for future projects

External Resources (if any):
Flex documentations
regex.com used for checking my rules


Quesitons as developing code:
forward slash? what is it do? how does it look ahead in flex?
using . for unknown, but that's only one character? will catch all?

