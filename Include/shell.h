/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell.h                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/17 16:53:36 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/17 16:53:36 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */


#ifndef SHELL_H
# define SHELL_H

# define BUFF_READ 4096

# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <stdbool.h>
# include "editor.h"
# include "../Libft/Includes/libft.h"

typedef struct				s_stdout
{
	BOOL				append;
	int 				from;
	char 				*to;
	struct s_stdout		*next;
	struct s_stdout		*start;
}							t_stdout;

/*
** Le premier maillon de t_cmd est vide et est pointé par tous les autres
** {hrdc[i] et std_in[i]} peuvent valoir soit une string, soit -1 si non rempli
** soit -2 si la chaine de char se trouve dans le prochain arg.
** Dans shell_hrdc, {hrdc[i]} prendra la valeur de -3 pour être indiqué
** comme rempli par l'utilisateur.
** hrdc_stdin peut avoir comme valeur -1 lors du parsing pour attendre d'être
** rempli
*/

typedef struct				s_cmd
{
	char 				*exec;
	char 				**args;
	t_stdout			*std_out;
	char 				**std_in;
	char 				**hrdc;
	char 				*hrdc_stdin;
	int 				sep;
	struct s_cmd		*next_cmd;
	struct s_cmd		*start;
}							t_cmd;

typedef struct				s_shell
{
	char 				**envp;
	char 				*str;
	t_history			*hist;
	char 				*cur_dir_run;
}							t_shell;

typedef enum 				e_sep
{
	SPL_PIPE = 1,
	PTN_VRGL = 2,
	DBL_PIPE = 3,
	DBL_SPRLU = 4,
}							t_sep;

typedef enum 				s_error
{
	SPL_QUOTE,
	DBL_QUOTE,
	DBL_SEP_OR_BK,
}							e_error;

void	builtin_cd(char **cmd, char ***envp);
void	builtin_setenv(char ***envp, char *key, char *value);
void	builtin_unsetenv(char ***envp, char *key);
void	builtin_delenv(char ***envp, char *key);
char	**rmv_key_env(char **envp, char *key);
void	builtin_env(char ***envp, char *key);
void	builtin_echo(char **cmd);

int		shell_error(char *type, int n, ...);

int		shell_builtin(t_cmd *link, t_shell *shell);
char	*get_envp(char **envp, char *var);
char	*get_var(char *var_key);
char	*get_cur_dir(void);
int		shell_argsub_env(char **arg, int i, char **envp);
void	*shl_mlc(char *type, int n, ...);

/*
**┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
**┃                                  Tools                                     ┃
**┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

t_shell		*init_shell(char **envp);
void		clean_shell(t_shell **shell);
int			ft_isfile(char *file_path);
int 		ft_isdir(char *path);

/*
**┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
**┃                               split_shell                                  ┃
**┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

t_cmd		*shell_split(char *line, char **envp, e_prompt *prompt);
t_cmd 		*get_args(char **line, char **envp, e_prompt *prompt);
void		shell_envpsub(char **arg, char **envp);
int 		shell_process(t_cmd **cmd, t_shell *shell);
t_stdout	*shell_std_out(char **arg, t_stdout **first_redi, char quote);
void		shell_std_in(char **arg, char quote, char ***ptn_stdin,
							char ***ptn_hrdc, char **hrdc_stdin);

BOOL		cmd_check(t_cmd **cmd, t_shell *shell, e_prompt *prompt);
int			hrdc_fill(e_prompt *prompt, t_cmd **cmd, t_shell *shell,
						e_shortcut ret);

size_t		len_arg(char *str, char quote);
int			clean_cmd(t_cmd **cmd);
char		*shell_trim(char **str);
int			check_last_quote(char *arg, char quote);
void		shl_quotesub(char *arg);
BOOL		stdout_to(t_stdout *redis);
int			len_stdout_to(char *str, char quote);
t_stdout	*get_last_stdout(t_stdout *redi);
char		*complete_stdout_to(char **arg, t_stdout *add_to, char quote);
void		complete_stdin(char **arg, char quote, char ***std_in);
int			shell_stdin_sub(char **arg, int i, char ***std_in);
char		**add_stdin(char **hrdc);
int 		shell_clean_data(t_cmd **cmd, t_shell *shell, BOOL t_cmd,
							BOOL shl_str);
int			len_stdin(char *str, char quote);

void		read_lexing(t_cmd *cmd);

/*
**┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
**┃                                 Process                                    ┃
**┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

int			shell_prepare(t_cmd *cmd, t_shell *shell);
char		*shell_getpathexec(char *exec, char **envp);
void		shell_clean_emptyargs(t_cmd *link);
int			complete_stdout_path(t_stdout *std_out, t_shell *shell);
void 		complete_stdin_path(char **std_in, t_shell *shell);
int			shell_error_prepare(char *msg, char *elem);

/*
** Hard test
** <  echo ~ ~te~st" ~ '$USER  \""+\\$USER+$US\ER~$USERS' ~ t"e$USER \'~'' ""'`' ""' \' ""'" \'>
** <echo "test>
** <echo test\ {ENTER} ' {ENTER} test {ENTER} '>
** <echo 'test\'
** echo tes't $USER te'st
** echo tes"t $USER te"st
** lancer minishell et <cd -> bash: cd: OLDPWD not set
** path qui contient un lien et <cd -L -L -P -L .> && <cd -L -P .>
** <mkdir test1> && <chmod 666 test1> && <cd test> --> Fail
** <mkdir test2> && <chmod 111 test2> && <cd test2> --> OK
** mkdir folder && cd folder && cd .. && rm -rf folder && cd -
** <cd \./> && pwd --> PWD et OLDPWD devrait avoir la meme valeur
** <cd \/.///> && env PWD && cd ..
** <cd ~///./folder//.//>
** cat * | ./minishell
** env -i ./minishell && unsetenv PATH && echo $HOME && cd ~
** ./minishell && unsetenv HOME && cd $random --> HOME not set
** ./minishell < "n'importe quel fichier"
** </> && <~> && </Users>
** ./minishell && ./minishell && ./minishell && CtrC && CtrlD && exit
** echo `ls\` --> ` && echo `ls\``
** echo "text" > file ; < file cat
** {echo test 1>/dev/ttys001 1>&2} --> la derniere redi est prit en compte et print test
** {cat missing 2>&1 1>/dev/ttys001} --> les redis sont save
** cat << EOF {ENTER} word1 {ENTER} word2 EOF {ENTER} EOF {ENTER}
** cat << EOF existing_file {ENTER} word1 {ENTER} EOF {ENTER}
** echo test >'&2'
** echo file > '&'
** {export tty=/dev/ttys001} {echo test > $tty} et car ttys001 recoit
** {echo test > "/dev/ttys001\\"} --> error avec {/dev/ttys001\\}
** echo test > file > /dev/ttys001 (le dernier est prit en compte mais file est créé)
** echo test > glob"{\n}file"
** echo test > file1>file2
** echo test > file1\>file2
** echo test > file1\\>file2
** echo test && {ENTER} \ {ENTER} \\ {ENTER} puis flèche du haut et histo == {echo test && \\}
** ; puis ;; (pas le meme msg d'erreur)
** {t &&} --> prompt
** >>>
** mkdir ~/folder && cd ~/folder && chmod 111 ~/folder && ~/21sh/./21sh && echo file_not_found > file
** << EOF cat nofile ;; --> les EOF puis ;; puis erreur de cat
** ;; "test {ENTER} " --> les fermetures des quotes sont prio face au ;;
** echo test \1>/dev/ttys00\2 '1>/dev/ttys003'
** echo test > file && cat < file>>file2
** a=5 b=3 echo $a (variable local ignoré)
** echo test << "1" && test ; <<\2
** cat << "EO {ENTER} F" {ENTER} puis essayer de fermer
** heredoc puis Cltr-c et Ctrl-v
** cat <<t {ENTER} test {Ctrl-D} ->> heredoc stop mais test dans cat
** << \'"test"\\
** file avec du txt dedans puis "> file" --> txt écrasé
** <<EOF<file_stdin
** <<EOF>file_stdout
** {test && w} et {test || w}
** env -i env
** exit | ls --> ne doit pas exit
** setenv VAR file && echo no_var>'$VAR' && echo var>$VAR
** setenv VAR ambiguous_redirect > $VAR
** export VAR file && echo test >~/$VAR
** unsetenv $HOME && echo "pwd.h get the *pw_name" >~/file
** echo file_line > file && cat << EOF1 < file << EOF2 puis line1 \n EOF1 \n line2 \n EOF2 --> dernier element avec bash et en suivant l'ordre pour zsh
** exit 1arg 2arg --> no exit
*/

/*
** HARD TEST (et inutile)
** echo ~root
** exit 30 {ENTER} echo $? -> valeur 30 et 1 pour {exit 257}
** exit 2>file
*/

#endif
