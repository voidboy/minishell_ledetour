echo bonjour ; |
# bash: syntax error near unexpected token `|'
# ERROR PARSER commande attendu avant |
|
# bash: syntax error near unexpected token `|'
# ERROR PARSER commande attendu avant |
echo bonjour |;
# bash: syntax error near unexpected token `;'
# ERROR PARSER commande attendu apres |
echo bonjour > test\ 1
# cat test\ 1 => bonjour
# WARNING PARSER a \
cd $HOME/Documents
# pwd /Users/etakouer/Documents
# WARNING EXPANSION expansion $ avant d'executer la commande
echo >
# bash: syntax error near unexpected token `newline'
# ERROR PARSE commande attendu apres >
echo -n -n -nnnn -nnnnm
# affiche -nnnnm sans retour a la ligne
# WARNING ECHO tq parm valide je consomme sinon j'affiche
unset jkjkj
# WARNING UNSET pas d'erreur si l'identifiant undefined
unset ""
# bash: unset: `': not a valid identifier
# ERROR UNSET EXPORT identfiant valide [a-zA-Z_]+[a-zA-Z0-9_]
$
# bash: $: command not found
# WARNING EXPANSION si $<identifiant> alors expension sinon rien faire
echo test > file test1
# cat file => test test1
# WARNING REDIRECTION >[|]word
echo bonjour >>> test
# bash: syntax error near unexpected token `>'
# ERROR PARSE commande attendu apres >>
 echo bonjour > > out
# bash: syntax error near unexpected token `>'
# ERROR PARSE commande attendu apres >
 echo 2 >> out1 > out2
 # cat out1 out2 => out1:vide out2:2
 # WARNING REDIRECTION open files before execute commande and take last *pour l'instant
 > log echo coucou
 # cat log => coucou
 # WARNING REDIRECTION la redirection est prise en compte n'importe ou
 cd ~
 cd /
cd a b c d
# bash: cd: a: No such file or directory
# ERROR CD pas de fichier
export "HI =hi"
# bash: export: `=hi': not a valid identifier
# ERROR EXPORT <identifier>=<string>
cat < >
# syntax error near unexpected token `>'
# ERROR REDIRECTION pas de fichier

