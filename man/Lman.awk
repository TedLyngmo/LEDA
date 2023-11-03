

function underline(s)
{ gsub(/./,"_&",s); 
  return s;
}


function header(s)
{ print "\n";
  print underline(sprintf("%d. %s", ++count,s)) "\n";
}


function sub_tex()
{ gsub(/\\{/,"kl_auf");
  gsub(/\\}/,"kl_zu");
  gsub(/{/,"");
  gsub(/}/,"");
  gsub("kl_auf", "{");
  gsub("kl_zu", "}");
  gsub(/\$/,"");
  gsub(/\\precond/,"Precondition:");
  gsub(/\\longrightarrow/, "---->");
  gsub(/\\phantom/, "");
  gsub(/\\times/, " x");
  gsub(/\\dots/, "...");
  gsub(/\\cdot/, " *");
  gsub(/\\tilde/,"~");
  gsub(/\\le/,   " <=");
  gsub(/\\ge/,   " >=");
  gsub(/\\neq/,  " !=");
  gsub(/\\ne/,   " !=");
  gsub(/\\bf /, "");
  gsub(/\\nl/, "");
  gsub(/\\/, "");
  gsub(/backslash /, "\\");
}


BEGIN { arg =  ARGV[2]; 
        type = ARGV[1];
        gsub(/\/.*\//, "",type);
        gsub(/\.tex/, "",type);
        ARGV[2] = ""; 
        gsub(/_/, "\\_", arg); print " "; 
        op_header = 0;
       }


/\\section/ { 
if (arg == "")
 { gsub(/\\section *{/, "");
   gsub(/\\label *\{.*\}/, "");
   gsub(/\}/, "");
   gsub(/\\/, "");
   print underline($0);
   print " ";
  }
}

/\\definition/ { 
if (arg == "" || "definition" == arg)
 { if (arg == "") header("DEFINITION");
    getline;
    while ($1 == "") getline;
    if ($1 ~ "\\\\decl") 
    { if ($1 == "\\declare")
        type = $2;
      else
        if ($1 == "\\declone")
          type = $2"<"$3">";
        else
          if ($1 == "\\decltwo")
             type = $2"<"$3","$4">";
          else
             if ($1 == "\\declthree")
                type = $2"<"$3","$4","$5">";
     getline;
    }
    while ($1 == "") getline;
    while ($1 != "")
      { gsub(/\\name/,type);
        sub_tex();
        print;
        getline; 
       }
    print " ";
  }
}


/\\creation/ { 
  out = 0;
  if (arg == "" || "creation" == arg) 
  { out = 1
    if (arg == "") header("CREATION");
   }
  getline;
  while ($1 == "") getline;

  while ($0 ~ "\\\\create") 
  { i = 1;
    while ($i != "\\create") i++;
    i++;
    var = $i; 
    gsub(/\\create/, type);
    gsub(/ *\{ *\} */, "");
    gsub(/\\/, "");
    if (out==1) printf("%s;\n\n",$0);
    getline;
    while ($1 == "") getline;
  }

  if (out==1) 
  { while ($1 != "")
    { gsub(/{/,"");
      gsub(/}/,"");
      gsub(/\\tt /, "");
      gsub(/\$/,"");
      gsub(/\\name/,type);
      gsub(/\\var/,var);
      gsub(/\\precond/, "Precondition:");
      gsub(/\\/,"");
      print;
      getline; 
     }
   print " ";
  }
}


#operations & operators: line starts with \op,\opl,\binop,\unop,\funop, ...

/\\operations/ {
  if (arg == "") header("OPERATIONS"); 
  getline; 
 }

/\\[a-z]*op/ {
  if ($3 == arg || arg == "" || "operations" == arg)
  { 
    sub_tex();
  
    if ($1 == "op" || $1 == "opl") #operation
     { long = 0;
       if ($1 == "opl") long = 1;
       printf("%-9s %s.%s (",$2,var,$3);
       for (i=4;i<=NF;i++) printf("%s ",$i);
       printf(")");
      }
    else
       if ($1 == "binop") #binary operator
        { printf("%-9s %s ",$2,var);
          for (i=3;i<=NF;i++) printf("%s ",$i);
         }
       else  
          if ($1 == "strop") #stream operator
           { printf("%-9s %s %s %s",$2,$4,$3,var);
             for (i=5;i<=NF;i++) printf("%s ",$i);
            }
          else
             if ($1 == "funop") #function call operator  
             { printf("%-9s %s (",$2,var);
               for (i=3;i<NF;i++) printf("%s ",$i);
               printf("%s)",$i);
              }
             else
               if ($1 == "unop") #unary operator
                 printf("%-9s %s%s",$2,$3,var);
               else
               if ($1 == "arrop") #array access operator
                 printf("%-9s %s [%s %s]",$2,var,$3,$4);
  
    #if (long == 1) print " ";   # space between call sequence and description
  
    getline;
    print " ";
    if ("operations" != arg)
    { while ($1 != "")
      { gsub(/\\var/, var);
        sub_tex();
        printf("                       ");
        for (i=1;i<=NF;i++) printf("%s ",$i);
        print " ";
        getline; 
       }
     print " ";
     print " ";
    }
  }
}



#/\\iteration/ { 
#if (arg == "" || "iteration" == arg) 
#{ if (operation == "") header("ITERATION");
#  getline;
#  while ($1  ~ "skip" || $1 == "") getline;
#  while ($1 !~ "skip")
#    { sub_tex();
#      print;
#      getline; 
#      while ($1 ~ "phantom" || $1 == "") getline;
#     }
#  print " ";
#}
#}

/\\implementation/ { 
  if (arg == "" || "implementation" == arg) 
  { if (arg == "") header("IMPLEMENTATION");
    getline;
    while ($1  ~ "skip" || $1 == "") getline;
    while ($1 !~ "skip" && $1 != "")
      { gsub(/{/,"");
        gsub(/}/,"");
        sub_tex();
        print;
        getline; 
       }
    print " ";
   }
}
