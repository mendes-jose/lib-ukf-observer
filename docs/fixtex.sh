#!/bin/sh

LANG=en_US.UTF-8
NOW=$(date +"%B %Y")

#Sacaling doxygen figures
find ./*.tex -type f -exec sed -i 's/width=350pt/width=\\textwidth/' {} \;
#Cover page and footnote
sed -i 's/ by Doxygen//g' refman.tex
sed -i '/\\begin{titlepage}/,/\\end{titlepage}/d' refman.tex
sed -i 's/\\pagenumbering{roman}/\\pagenumbering{roman}\r\n\\begin{titlepage}\r\n\\vspace*{7cm}\r\n\\begin{center}\r\n{\\Huge Sat\\-Com and 3\\-G Library -\\\/ D\\-E\\-R\\-I\\-C }\\\\\r\n\\vspace*{1cm}\r\n{\\small '"$NOW"'}\\\\\r\n\\end{center}\r\n\\vspace*{2.5cm}\r\n\\begin{figure}[h]\r\n\t\\centering\r\n\t\\includegraphics[scale=.3]{..\/..\/docs\/images\/Delai1133_logofinal.png}\r\n\\end{figure}\r\n\\begin{figure}[h]\r\n\t\\centering\r\n\t\\includegraphics[scale=.9]{..\/..\/docs\/images\/magellium_logo.png}\r\n\\end{figure}\r\n\\begin{figure}[h]\r\n\t\\centering\r\n\\includegraphics[scale=.3]{..\/..\/docs\/images\/Airbus_Defence_Space_logo_2014.png}\r\n\\end{figure}\r\n\\end{titlepage}/' refman.tex

##Search for inserted figures
#find ./*tex -type f -exec sed -i 's/\(\$>\$\\}\\{\)/\r\n\1/g;s/\(\\}\\{\$<\$\)/\1\r\n/g' {} \;
#find ./*tex -type f -exec sed -i 's/\$>\$\\}\\{\([^;]*\)\/\([^.]*\).\([^;]*\);\([^.]*\).\\-\([^;]*\);\([^\0-9]*\)\([^0-9]*\)\([^;]*\);\([^$]*\)\\}\\{\$<\$/\\begin{figure}[H]\r\n\t\\centering\r\n\t\\includegraphics[angle=\6\8,width=\4.\5\\textwidth]{\1\/\2.\3}\r\n\t\\caption{\9\\label{fig:\2}}\r\n\\end{figure}\r\n/g' {} \;
##Search for references
#find ./*.tex -type f -exec sed -i 's/\\-:\\}\\{\([^\]*\)\\-:\([^\]*\)\\}\\{\\-:/\\ref\{\1:\2\}/g' {} \;

#sed -i 's/\\begin{DoxyParagraph}{figuretlc}/figuretlc\r\n\\begin{DoxyParagraph}{figuretlc}/' telecom_8h.tex
#sed -i '/\\begin{DoxyParagraph}{figuretlc}/,/\\end{DoxyParagraph}/d' telecom_8h.tex
#sed -i 's/figuretlc/\\begin{figure}[H]\r\n\t\\centering\r\n\t\\includegraphics[angle=-90,width=1.1\\textwidth]{..\/..\/docs\/images\/shc0.pdf}\r\n\t\\caption{Library application flowchart part 1\\label{fig:flowchartp1}}\r\n\\end{figure}\r\n\\begin{figure}[H]\r\n\t\\centering\r\n\t\\includegraphics[angle=-90,width=1.1\\textwidth]{..\/..\/docs\/images\/shc3.pdf}\r\n\t\\caption{Library application flowchart part 2\\label{fig:flowchartp2}}\r\n\\end{figure}/' telecom_8h.tex
#sed -i 's/\\begin{titlepage}\(.*\)\\end{titlepage}/\\begin{titlepage}\r\n\\vspace*{7cm}\r\n\\begin{center}\r\n{\\Huge Sat\\-Com and 3\\-G Library -\\\/ D\\-E\\-R\\-I\\-C }\\\\\r\n\\vspace*{1cm}\r\n{\\small December 2013}\\\\\r\n\\end{center}\r\n\\vspace*{2.5cm}\r\n\\begin{figure}[h]\r\n\t\\centering\r\n\t\\includegraphics[scale=.3]{..\/..\/docs\/images\/Delai1133_logofinal.png}\r\n\\end{figure}\r\n\\begin{figure}[h]\r\n\t\\centering\r\n\t\\includegraphics[scale=.9]{..\/..\/docs\/images\/magellium_logo.png}\r\n\\end{figure}\r\n\\begin{figure}[h]\r\n\t\\centering\r\n\\includegraphics[scale=.068]{..\/..\/docs\/images\/EADS-Astrium.png}\r\n\\end{figure}\r\n\\end{titlepage}\r\n/g' refman.tex
#sed -i 's/titlepage\(.*\)titlepage/asdf/' refman.tex

