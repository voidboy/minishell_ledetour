# la sequence \\ donne \
echo "\\"
# \
export ARG="\\" ; env | grep ARG
# ARG=\
echo \\
# \
export ARG=\\ ; env | grep ARG
# ARG=\
#########################
echo "\\\\"
# \\
export ARG="\\\\" ; env | grep ARG
# ARG=\\
echo \\\\
# \\
export ARG=\\\\ ; env | grep ARG
# ARG=\\
#########################
echo "\\\\\\"
# \\\
export ARG="\\\\\\" ; env | grep ARG
# ARG=\\\
echo \\\\\\
# \\\
export ARG=\\\\\\ ; env | grep ARG
# ARG=\\\
#########################
# La sequence \\ donne \, si la sequence a echappee est invalide,
# l'antislash est affiche suivi du caractere non echappable
echo "\\\0"
# \\0
export ARG="\\\0" ; env | grep ARG
# ARG=\\0
# IDENTIFIER CE CAS
echo \\\0
# \0
export ARG=\\\0 ; env | grep ARG
# ARG=\0
#########################
echo "\\\\0"
# \\0
export ARG="\\\\0" ; env | grep ARG
# ARG=\\0
echo \\\\0
# \\0
export ARG=\\\\0 ; env | grep ARG
# ARG=\\0
echo "\\\\\0"
# \\\0
export ARG="\\\\\0" ; env | grep ARG
# ARG=\\\0
echo \\\\\0
# \\0
export ARG=\\\\\0 ; env | grep ARG
# ARG=\\0
#########################
echo "\\\\\\0"
# \\\0
export ARG="\\\\\\0" ; env | grep ARG
# ARG=\\\0
echo \\\\\\0
# \\\0
export ARG=\\\\\\0 ; env | grep ARG
# ARG=\\\0
# Double quotes(escaped) insides double quotes are ALLOWED
# Single quotes(escaped) insides signle quotes are FORBIDN
# Tripouille failled seq.
echo "test\"test" \\\0\"\'
test"test \0"'
