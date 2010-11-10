Needlestack
===========

Needlestack is a static web file server that serves file stacks (as opposed
to individual files).  It is based loosely (heavily) (entirely) upon Facebook's
Haystack, with notable differences being the lack of an explicit in-file index
as well as the use of a (poorly implemented) trie isntead of Google's sparse
hash table.

Example Use:
> \# To make a stack with a directory (pics)  
> python scripts/makestackfromdir.py pics.stack pics
>  
> \# To serve up the pics stack on port 12345  
> needlestack -f pics.stack -p 12345  

Dependencies:  
- libevent (for http serving)

Arguments:  
> \-f <stackfile>  stack file to use  
> \-p <port>       port on which to listen for requests  


## Facebook's Haystack ##

About: [Needle in a haystack](http://www.facebook.com/note.php?note_id=76191543919)

### Comparison of Needlestack and Haystack ###

- Needlestack does not currently support real-time append (but could)
- Needlestack's metadata is just size, uri, and content-type (as opposed to
  key, alternate key, etc.)
