Needlestack
===========

Needlestack is a static web file server that serves file stacks (as opposed
to individual files).  It is based loosely (heavily) (entirely) upon Facebook's
Haystack, with notable differences being the lack of an explicit in-file index
as well as the use of a (poorly implemented) trie isntead of Google's sparse
hash table.

Example Use:
> needlestack -f <stackfile.stack> [-p \<port\>]  
> needlestack -c -f <stackfile.stack> -b \<dir base\> \<path\> \<path\> ...  
> 
> \# To make a stack with a directory (mydir)  
> /bin/bash scripts/makestack.sh mystack.stack mydir  
>
> \# To serve that stack on port 6767  
> needlestack -f mystack.stack -p 6767  

Dependencies:  
- libevent (for http serving)



Arguments:  
> \-f <stackfile>  stack file to use  
> \-c              create a new stack file  
> \-b \<basedir\>  specify a base directory for stackfile creation  

