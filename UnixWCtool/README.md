## Unix WC Tool
`wc` - print newline, word, and byte counts for each file
Print newline, word, and byte counts for each FILE, and a total line if more than one FILE is specified.
With no FILE, or when FILE is -, read standard input.

`-c, --bytes`: print the byte counts

`-m, --chars`: print the character counts

`-l, --lines`: print the newline counts

`--files0-from=F`: read input from the files specified by NUL-terminated names in file F;
                 If F is - then read names from standard input
                 
`-L, --max-line-length`: print the length of the longest line

`-w, --words`: print the word counts


### wcTool_version3.cpp
**Step 1:**
In this step goal is to write a simple version of wc, that takes the command line option -c and outputs the number of bytes in a file.
>ccwc -c test.txt
  342190 test.txt

**Step2:**
In this step your goal is to support the command line option -l that outputs the number of lines in a file.
>ccwc -l test.txt
    7145 test.txt

**Step3:**
In this step your goal is to support the command line option -w that outputs the number of words in a file. If you’ve done it right your output should match this:
>ccwc -w test.txt
   58164 test.txt

**Step4:**
In this step your goal is to support the command line option -m that outputs the number of characters in a file. If the current locale does not support multibyte
characters this will match the -c option. For this one your answer will depend on your locale, so if can, use wc itself and compare the output to your solution:

>wc -m test.txt
  339292 test.txt

>ccwc -m test.txt
  339292 test.txt

**Step5:**
In this step your goal is to support the default option - i.e. no options are provided, which is the equivalent to the -c, -l and -w options. If you’ve done it right your output should match this:

>ccwc test.txt
  7145   58164  342190 test.txt

**Final Step:**
In this step your goal is to support being able to read from standard input if no filename is specified. If you’ve done it right your output should match this:

>cat test.txt | ccwc -l
    7145
