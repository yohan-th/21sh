/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell.h                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/24 21:34:32 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/06 15:03:34 by dewalter    ###    #+. /#+    ###.fr     */
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

typedef struct				s_cmd
{
	char 				**args;
	t_stdout			*std_out;
	char 				**std_in;
	char 				**hrdc;
	int 				sep;
	struct s_cmd		*next_cmd;
	struct s_cmd		*start;
}							t_cmd;

typedef struct				s_shell
{
	char 				**envp;
	char 				*str;
	t_history			*hist;
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

char	*path_cmd(char *cmd, char *envp_path);
void	builtin_cd(char **cmd, char ***envp);
void	builtin_setenv(char ***envp, char *key, char *value);
void	builtin_unsetenv(char ***envp, char *key);
void	builtin_delenv(char ***envp, char *key);
char	**rmv_key_env(char **envp, char *key);
void	builtin_env(char ***envp, char *key);
void	builtin_echo(char **cmd);

int		shell_error(char *type, int n, ...);

void	ft_arrdel(char **arr);
char	*check_builtin(char **cmd, char ***envp);
char	*get_envp(char **envp, char *var);
char	*get_var(char *var_key);
char	**ft_arrdup(char **arr);
void	ft_strdelchar(char **str, char c);
char	*get_cur_dir(void);
int		ft_arrlen(char **arr);
int		shell_argsub_env(char **arg, int i, char **envp);
void	*shl_mlc(char *type, int n, ...);

/*
**┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
**┃                                  Tools                                     ┃
**┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

t_shell	*init_shell(char **envp);
void	clean_shell(t_shell **shell);

/*
**┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
**┃                               split_shell                                  ┃
**┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

t_cmd		*shell_split(char *line, char **envp, e_prompt *prompt);
t_cmd 		*get_args(char **line, char **envp, e_prompt *prompt);
void		shell_envpsub(char **arg, char **envp, char quote);
void 		shell_process(t_cmd *cmd, t_shell *shell);
t_stdout	*shell_std_out(char **arg, t_stdout **first_redi, char quote);
char		**shell_heredoc(char **arg, char quote, char **hrdc);
char		**shell_std_in(char **arg, char quote, char **std_in);

size_t		len_arg(char *str, char quote);
void		clean_cmd(t_cmd **cmd);
char		*shell_trim(char **str);
int			check_last_quote(char *arg, char quote);
int			shl_quotesub(char *arg);
BOOL		stdout_to(t_stdout *redis);
int			len_stdout_to(char *str, char quote);
t_stdout	*get_last_stdout(t_stdout *redi);
char		*complete_stdout_to(char **arg, t_stdout *add_to, char quote);
void		complete_hrdc(char **arg, char quote, char ***hrdc);
int			shell_hrdc_sub(char **arg, int i, char ***hrdc);
char		**add_hrdc(char **hrdc);

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
** <cat > file << EOF> \n <line> <\n> <EOF>
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
** >>>
** mkdir ~/folder && cd ~/folder && chmod 111 ~/folder && ~/21sh/./21sh && echo file_not_found > file
** << EOF cat nofile ;; --> les EOF sont prio puis ;; puis erreur de cat
** echo test \1>/dev/ttys00\2 '1>/dev/ttys003'
** echo test > file && cat < file>>file2
*/


#endif
