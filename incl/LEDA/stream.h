/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  stream.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/



#ifndef LEDA_STREAM_H
#define LEDA_STREAM_H

#include <LEDA/basic.h>


#if defined (__ZTC__)

#include <fstream>
#include <strstream>

/* bug in ZORTECH library ?
   cannot derive from streams */

typedef std::ifstream   file_istream;
typedef std::ofstream   file_ostream;
typedef istrstream string_istream;
typedef ostrstream string_ostream;


#else

#include <fstream>
#include <strstream>


/*{\Manpage {file_istream} {} {File Input Streams} }*/


struct file_istream : public std::ifstream {

/*{\Mdefinition
An instance $I$ of the data type $file\_istream$ is an \CC std::istream
connected to a file $F$, i.e., all input operations or operators
applied to $I$ read from $F$. }*/

/*{\Mcreation I }*/

file_istream(const char*  s) : std::ifstream(s) {}
file_istream(string s) : std::ifstream(~s) {}
/*{\Mcreate    creates an instance $I$ of type file\_istream connected to
               file $s$. }*/


/*{\Moperations 2 5.5 }*/

/*{\Mtext
All operations and operators ($>>$) defined for \CC std::istreams can
be applied to file input streams as well.
}*/

  bool open(string s)  { std::ifstream::open(~s); return !fail(); }

};




/*{\Manpage {file_ostream} {} {File Output Streams} }*/


struct file_ostream : public std::ofstream {

/*{\Mdefinition
An instance $O$ of the data type $file\_ostream$ is an \CC std::ostream
connected to a file $F$, i.e., all output operations or operators
applied to $O$ write to $F$. }*/

/*{\Mcreation O }*/

file_ostream(string s) : std::ofstream(~s) {}
file_ostream(char*  s) : std::ofstream(s) {}
/*{\Mcreate    creates an instance $O$ of type file\_ostream connected to
               file $s$.}*/


/*{\Moperations 2 5.5 }*/

/*{\Mtext
All operations and operators ($<<$) defined for \CC std::ostreams can
be applied to file output streams as well.
}*/

 bool open(string s)  { std::ofstream::open(~s); return !fail(); }

};




/*{\Manpage {string_istream} {} {String Input Streams} }*/

struct string_istream : public istrstream {

/*{\Mdefinition
An instance $I$ of the data type $string\_istream$ is an \CC std::istream
connected to a string $s$, i.e., all input operations or operators
applied to $I$ read from $s$. }*/

/*{\Mcreation I }*/


string_istream(char* s)  : istrstream(s) {}
string_istream(string s) : istrstream(~s) {}
/*{\Mcreate   creates an instance $I$ of type string\_istream connected to
              the string $s$. }*/

string_istream(int argc, char** argv) : istrstream(~string(argc,argv)) {}


/*{\Moperations 2 5.5 }*/

/*{\Mtext
All operations and operators ($>>$) defined for \CC std::istreams can
be applied to string input streams as well.
}*/


 };





/*{\Manpage {string_ostream} {} {String Output Streams} }*/

struct string_ostream : public ostrstream {

/*{\Mdefinition
    An instance $O$ of the data type $string\_ostream$ is an \CC std::ostream
    connected to an internal string buffer, i.e., all output operations
    or operators applied to $O$ write into this internal buffer.  The current
    value of the buffer is called the contents of $O$.}*/


/*{\Mcreation O }*/

string_ostream() {};
/*{\Mcreate creates an instance $O$ of type string\_ostream.}*/


/*{\Moperations 2 5.5 }*/

string str()     { return ostrstream::str(); };
/*{\Mop   returns the current contents of $O$.}*/


/*{\Mtext
All operations and operators ($<<$) defined for \CC std::ostreams can
be applied to string output streams as well.
}*/

};



#endif


#if !defined(unix) || defined(__lucid)

typedef file_ostream cmd_ostream;
typedef file_istream cmd_istream;

#else


#include <stdio.h>


/*{\Manpage {cmd_istream} {} {Command Input Streams} }*/


struct cmd_istream : public std::ifstream
{

/*{\Mdefinition
An instance $I$ of the data type $cmd\_istream$ is an \CC std::istream
connected to the output of a shell command $cmd$, i.e., all input operations
or operators applied to $I$ read from the standard output of command $cmd$. }*/

/*{\Mcreation I }*/


  cmd_istream(char*  cmd) : std::ifstream(fileno(popen(cmd,"r"))) {}
  cmd_istream(string cmd) : std::ifstream(fileno(popen(~cmd,"r"))) {}

/*{\Mcreate   creates an instance $I$ of type cmd\_istream connected to the
              output of command $cmd$. }*/


/*{\Moperations 2 5.5 }*/

/*{\Mtext
All operations and operators ($>>$) defined for \CC std::istreams can
be applied to command input streams as well.
}*/

 };





/*{\Manpage {cmd_ostream} {} {Command Output Streams} }*/

struct cmd_ostream : public std::ofstream {

/*{\Mdefinition
An instance $O$ of the data type $cmd\_ostream$ is an \CC std::ostream
connected to the input of a shell command $cmd$, i.e., all output operations
or operators applied to $O$ write into the standard input of command $cmd$. }*/

/*{\Mcreation O }*/

cmd_ostream(const char*  cmd) : std::ofstream(fileno(popen(cmd,"w"))) {}
cmd_ostream(string cmd) : std::ofstream(fileno(popen(~cmd,"w"))) {}
/*{\Mcreate
creates an instance $O$ of type cmd\_ostream connected to the input of
command $cmd$.  }*/


/*{\Moperations 2 5.5 }*/

/*{\Mtext
All operations and operators ($<<$) defined for \CC std::ostreams can
be applied to command output streams as well.
}*/

};

#endif

#endif

