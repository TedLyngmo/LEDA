BEGIN { datatype = "?????????????"; }

function removeweb() 
{ x = 0; 
# gsub(/\|.*\|/,"$DUMMY$");
# gsub("_","\\\\_");
}


/\/\*{\\Moperations/ { 
  sub(/\/\*{/,"");
  sub(/}\*\//,"");
  sub("Moperations","operations");
  print;
  print " ";
}

/\/\*{\\Mop/ {
  if ($1 ~ "opl")
     print "\\opl " memberfunc;
  else
     print "\\op  " memberfunc;
  $1 = "           { ";
  while ($0 !~ /\} *\*\//)
  { removeweb();
    print;
    getline;
   }
  sub(/} *\*\//," }");
  removeweb();
  print;
  print " ";
  print " ";
  getline;
}

/\/\*{\\Mfunc/ {
  if ($1 ~ "funcl")
     print "\\funcl " memberfunc;
  else
     print "\\func  " memberfunc;
  $1 = "           { ";
  while ($0 !~ /\} *\*\//)
  { removeweb();
    print;
    getline;
   }
  sub(/} *\*\//," }");
  removeweb();
  print;
  print " ";
  print " ";
  getline;
}

/\/\*{\\Mbinopfunc/ {
  split(memberfunc,arr);
  if (arr[2] !~ /[a-z,A-Z]+.*/)   # binary operator
  { sub(arr[2],"",memberfunc);
    sub(/\(/,"",memberfunc);
    sub(/\)/,"",memberfunc);
    sub(/\,/,"\\ "arr[2],memberfunc);
   }
  rtype = arr[1]
  sub(/\\/,"\\\\",rtype);
  sub(rtype,"",memberfunc);
  gsub(datatype,"",memberfunc);
  gsub(/ \\ /,"",memberfunc);
  sub(/ \&/," \\\\&",memberfunc);
  print "\\binopfunc  " arr[1] " " memberfunc;
  $1 = "           { ";
  while ($0 !~ /\} *\*\//)
  { removeweb();
    print;
    getline;
   }
  sub(/} *\*\//," }");
  removeweb();
  print;
  print " ";
  print " ";
  getline;
}



/\/\*{\\Marrop/ {
  sub(/\[\]/,"",memberfunc);
  sub(/\(/,"",memberfunc);
  sub(/\)/,"",memberfunc);
  print "\\arrop " memberfunc;
  $1 =  "         { ";
  while ($0 !~ /} *\*\//)
  { removeweb();
    print;
    getline;
   }
  sub(/} *\*\//," }");
  removeweb();
  print;
  print " ";
  print " ";
  getline;
}

/\/\*{\\Mfunop/ {
  gsub(/\(/,"",memberfunc);
  gsub(/\)/,"",memberfunc);
  print "\\funop " memberfunc;
  $1 =  "         { ";
  while ($0 !~ /} *\*\//)
  { removeweb();
    print;
    getline;
   }
  sub(/} *\*\//," }");
  removeweb();
  print;
  print " ";
  print " ";
  getline;
}

/\/\*{\\Munop/ {
  if (memberfunc ~ /\( *int/)       # postfix op
    opcmd = "\\postunop ";
  else
    opcmd = "\\unop ";
  sub(/\(.*\)/,"",memberfunc);
  sub(/\{\}/,"",memberfunc);
  print opcmd memberfunc;
  $1 =  "         { ";
  while ($0 !~ /} *\*\//)
  { removeweb();
    print;
    getline;
   }
  sub(/} *\*\//," }");
  removeweb();
  print;
  print " ";
  print " ";
  getline;
}



/\/\*{\\Mbinop/ {
  dtype = datatype;
  sub(/\\/,"\\\\",dtype);
  gsub(dtype,"@",memberfunc); # delete all but first occurences of datatype
  sub(/^@/,datatype,memberfunc);
  gsub(/@/,"",memberfunc);
  sub(/\(/,"",memberfunc);
  sub(/\)/,"",memberfunc);
  gsub(/ \\ /,"",memberfunc);
  print "\\binop " memberfunc;
  $1 =  "         { ";
  while ($0 !~ /} *\*\//)
  { removeweb();
    print;
    getline;
   }
  sub(/} *\*\//," }");
  removeweb();
  print;
  print " ";
  print " ";
  getline;
}



/\/\*{\\Mcreate/ {
  print "\\create " constargs;
  $1 = "   {"
  while ($0 !~ /} *\*\//)
  { removeweb();
    print;
    getline;
   }
  gsub(/} *\*\//," }");
  removeweb();
  print;
  print " ";
  print " ";
  getline;
}


/\/\*{\\Manpage/ { 
  sub(/\/\*{\\Manpage/,"\\manpage");
  sub(/}\*\//,"");
  gsub("_","\\_");
  print;
  print " ";
  gsub("{"," ");
  gsub("}"," ");
  datatype = $2;
}



/\/\*{\\Mtext/ { 
  sub(/\/\*{\\Mtext/,"");
  while ($0 !~ /}\*\//) 
  { removeweb();
    print;
    getline;
   }
  sub(/}\*\//,"");
  removeweb();
  print;
  print " ";
}

/\/\*{\\M/ { 
  sub(/\/\*{\\M/,"\\");
  while ($0 !~ /}\*\//) 
  { removeweb();
    print;
    getline;
   }
  sub(/}\*\//,"");
  removeweb();
  print;
  print " ";
}


/^ *[a-z,A-Z,0-9,_,&,<,>]+ +[a-z,A-Z,0-9,_,&]+/ {  # member or friend function ?
sub(/ *virtual */,"");
sub("return","");
sub("friend","");
sub("inline","");
sub("extern","");
if ($0 ~ /^.*operator/ || $0 ~ /^ *[a-z,A-Z,0-9,_,&,<,>]+ +[a-z,A-Z,0-9,_]+\(.*\)/)
   { 
      sub(/ *\{.*$/,"");
      sub(/ +$/,"");
      sub(/;$/,"");
      sub(/^ +/,"");

      gsub("<","\\<",$1);   # replace template brackets in first arg
      gsub(">","\\>",$1);
      gsub("_","\\_");
      gsub("%","\\%");
      gsub("\\|","|");
      gsub("~","\\tilde{}");
      sub(/const *$/,"");
      sub(/operator */,"operator");
      sub(/\(/," {(");
      sub(/\) *$/,")}");
      if ($0 !~ /\(\*.+\)/)
        { #gsub(/\, */,"\, ");
          for(i=2;i<NF;i++) sub(/\, */,"\, ",$i);
          for(i=3;i<NF;i++) 
          { if ($i ~ "const") 
              { j = i+1;
                sub("&","",$j);
                sub(/const */,"",$i); 
               }
            else
              sub(/$/,"\\",$i);
            gsub("<","\\<",$i);  # replace template brackets in parameters 
            gsub(">","\\>",$i);
           }
         }
      else
         for(i=3;i<NF;i++) sub(/$/,"\\",$i);
      sub(/operator/,"");
      gsub(/\&/,"\\\\&");

      memberfunc = $0;
     }
 }



{ if ($0 ~ datatype"\\(")  # constructor ?
  {
    gsub("_","\\_");
    gsub("&","");
    gsub(/const */,"");
    sub(/ *\{.*$/,"");
    sub(/::/,"doppeldoppel");
    sub(/ *:.*$/,"");
    sub("doppeldoppel","::");
    sub(/; *$/,"");
    sub(/\(/," {(");
    sub(/\) *$/,")}");
    sub(/^.*\{/,"{");
    sub(" ","\\ ");
    for(i=2;i<NF;i++) sub(/$/,"\\",$i);
    sub("\\(\\)"," ");
    gsub("<","\\<");
    gsub(">","\\>");
    constargs = $0
   }
}

