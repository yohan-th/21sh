/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   minishell.h                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/05/04 20:58:23 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/06/11 12:20:02 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H


# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>

# include "../Libft/Includes/libft.h"

typedef struct				s_cmd
{
	char 				**args;
	int 				sep;
	struct s_cmd		*next_cmd;
	struct s_cmd		*start;
}							t_cmd;

typedef struct				s_shell
{
	BOOL				mltline;
	char 				**envp;
	char 				*str;
}							t_shell;

typedef enum 				e_sepcmd
{
	SPL_REDI_D = 1,
	SPL_PIPE = 2,
	SPL_REDI_G = 3,
	PTN_VRGL = 4,
	DBL_PIPE = 50,
	DBL_REDI_G = 60,
	DBL_SPRLU = 70,
	DBL_REDI_D = 80,
}							t_sepcmd;

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
int		shell_error(char *type, int n, ...);

/*
**┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
**┃                                  Tools                                     ┃
**┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

t_shell	*init_shell(char **envp);
int		prompt(int multi);

/*
**┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
**┃                               split_shell                                  ┃
**┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

t_cmd	*shell_split(char *line, char **envp);
char 	**get_args(char **line, char **envp);
void	shell_envpsub(char **arg, char **envp);
void 	shell_process(t_cmd *cmd, t_shell *shell);

void	clean_cmd(t_cmd **cmd);
char	*shell_trim(char **str);
int		check_last_quote(char *arg, char quote);


/*
** Hard test
** <  echo ~ ~te~st" ~ '$USER  \""+\\$USER+$US\ER~$USERS' ~ t"e$USER \'~'' ""'`' ""' \' ""'" \'>
** <echo "test>
** <echo test\>
** <echo 'test\'
** lancer minishell et <cd -> bash: cd: OLDPWD not set
** path qui contient un lien et <cd -L -L -P -L .> && <cd -L -P .>
** <mkdir test1> && <chmod 666 test1> && <cd test> --> Fail
** <mkdir test2> && <chmod 111 test2> && <cd test2> --> OK
** <cd \./> && pwd
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
*/

#endif
