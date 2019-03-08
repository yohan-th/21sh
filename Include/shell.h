/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell.h                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/17 16:53:36 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2019/03/04 17:47:03 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# define BUFF_READ 2

# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <signal.h>
# include "editor.h"
# include "../Libft/Includes/libft.h"

typedef struct		s_process
{
	char	*fd_stdin;
	char	*stdin_send;
	char	*fd_stdout;
	int		fd_fileout;
	char	*fd_stderr;
	int		fd_fileerr;
}					t_process;

typedef struct		s_stdout
{
	BOOL				append;
	int					from;
	char				*to;
	struct s_stdout		*next;
	struct s_stdout		*start;
}					t_output;

/*
** Le premier maillon de t_cmd est vide et est pointé par tous les autres
** {hrdc[i] et std_in[i]} peuvent valoir soit une string, soit -1 si non rempli
** soit -2 si la chaine de char se trouve dans le prochain arg.
** Dans shell_hrdc, {hrdc[i]} prendra la valeur de -3 pour être indiqué
** comme rempli par l'utilisateur.
** hrdc_stdin peut avoir comme valeur -1 lors du parsing pour attendre d'être
** rempli
*/

typedef struct		s_cmd
{
	char			*exec;
	char			**args;
	t_output		*output;
	char			**input;
	char			**hrdc;
	t_process		process;
	int				sep;
	int				ret;
	struct s_cmd	*next_cmd;
	struct s_cmd	*start;
}					t_cmd;

typedef struct		s_type
{
	int		i;
	int		j;
	int		match[2];
	char	*op;
	char	*b_path;
	char	**bin;
}					t_type;

typedef struct		s_shell
{
	char	**envp;
	char	**envl;
	char	**alias;
	char	*str;
	char	*hrdc_tmp;
	char	*hist_path;
	int		ret;
	t_data	*hist;
}					t_shell;

typedef enum		e_sep
{
	SPL_PIPE = 1,
	PTN_VRGL = 2,
	DBL_PIPE = 3,
	DBL_SPRLU = 4,
}					t_sep;

typedef enum		e_error
{
	SPL_QUOTE,
	DBL_QUOTE,
	DBL_SEP_OR_BK,
}					t_error;

/*
**┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
**┃                                  Builtin                                   ┃
**┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

int					builtin_cd(char **cmd, char ***envp);
char				*cd_rmv_last_path(char *cur_dir);
void				builtin_setenv(char ***envp, char *key, char *value);
void				builtin_unsetenv(char ***envp, char *key);
void				builtin_delenv(char ***envp, char *key);
int					builtin_eewefnv(char ***envp, char ***envl, char **args);
char				**rmv_key_env(char **envp, char *key);
void				builtin_env(char **envp, char *key);
int					builtin_echo(char **cmd);
int					builtin_env_all(char ***envp, char ***envl, char **args);
int					builtin_exit(char **cmd);

int					builtin_type(char **args, char **envp);
int					builtin_get_options(char **options, char **args,
											char *possibility);
int					builtin_type_check_builtin(char *d_name);
void				builtin_type_display(char *d_name, char *bin,
											char *options, int mode);
int					builtin_alias(char ***alias, char **args);
char				**builtin_alias_get_alias_from_file(char *file);
int					check_executable_file(char *path);
int					builtin_unalias(char ***alias, char **args);

int					shell_builtin(t_cmd *elem, t_shell *shell);
char				*get_envp(char **envp, char *var);
char				*get_var(char *var_key);
char				**append_key_env(char **envp, char *key, char *value);
int					check_replace_env_variable(char ***env, char *var,
													char *value);

/*
**┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
**┃                               split_shell                                  ┃
**┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

t_cmd				*shell_split(char *line, char **envp, t_prompt *prompt);
t_cmd				*get_args(char **line, char **envp, t_prompt *prompt);
void				shell_envpsub(char **arg, char **envp, char **envl);
int					shell_argsub_env(char **arg, int i, char **envp,
										char **envl);
int					shell_process(t_cmd **cmd, t_shell *shell);
t_output			*shell_std_out(char **arg, t_output **first_redi,
										char quote);
void				shell_std_in(char **arg, char quote, t_cmd *cmd);

BOOL				cmd_check(t_cmd **cmd, t_shell *shell, t_prompt *prompt);
int					hrdc_fill(t_prompt *prompt, t_cmd **cmd, t_shell *shell,
						t_shortcut ret);

size_t				len_arg(char *str, char quote);
void				clean_cmd(t_cmd **cmd);
char				*shell_trim(char **str);
int					check_last_quote(char *arg, char quote);
BOOL				stdout_to(t_output *redis);
int					len_stdout_to(char *str);
t_output			*get_last_stdout(t_output *redi);
char				*complete_stdout_to(char **arg, t_output *add_to);
void				complete_stdin(char **arg, char quote, char ***std_in);
int					shell_stdin_sub(char **arg, int i, char ***std_in);
char				**add_stdin(char **hrdc);
int					shell_clean_data(t_cmd **cmd, t_shell *shell,
										BOOL hrdc_tmp);
int					len_stdin(char *str, char quote);
char				*get_stdout_to(char *redi, int pos);
void				read_lexing(t_cmd *elem);

/*
**┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
**┃                                 Process                                    ┃
**┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

void				shell_prepare(t_cmd *cmd, t_shell *shell);
char				*shell_getpathexec(char *exec, char **envp);
void				shell_clean_emptyargs(t_cmd *link);
int					complete_stdout_path(t_output *std_out, t_shell *shell);
int					shell_error_prepare(char *msg, char *elem);
int					shell_read_input(t_cmd *elem, t_shell *shell);
int					shell_set_output(t_cmd *elem, t_shell *shell);
void				shell_execve(t_cmd *elem, t_shell *shell);
int					shell_exec(t_cmd *elem, t_shell *shell);

void				shell_save_fd(int fd[3]);
void				shell_reinit_fd(int *fd);
void				shell_prcs_sigint(int signum);
int					ft_read_file(char *filename, char **file_content);
int					path_to_output_exist(char *output);
int					complete_output_paths(char **output_to, t_shell *shell);
int					path_to_output_recheable(char *output);
void				shell_plomberie(t_process process);

int					get_stdin(t_shell *shell, t_prompt *prompt);
int					shell_exec_pipes(t_cmd **elem, t_shell *shell);
void				shell_set_fd_null(t_output *output, t_cmd *elem);

int					get_nbarg(char *str, t_prompt *prompt);
char				*get_arg(char **str, t_cmd *cmd);

/*
**┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
**┃                                  Tools                                     ┃
**┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

t_shell				*init_shell(char **envp);
int					check_if_env_var_existing(char **env, char*var);
char				**init_env(char **envp);
int					shell_error(char *type, int n, ...);
int					shell_error_env(char *msg);
void				clean_shell(t_shell **shell);
int					ft_isfile(char *file_path);
int					ft_isdir(char *path);
char				*get_cur_dir(void);
void				shl_quotesub(char *arg);
void				*shl_mlc(char *type, int n, ...);
int					shell_exit(t_cmd **cmd, t_shell **shell);
void				shell_init(t_shell **shell, t_prompt *prompt, t_cmd **cmd,
						char **env);
char				*get_next_hrdc(char **hrdc);

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
** cd ~bocal/Public --> {~} correspond à /Users et {~/} correpond à /Users/ythollete
** echo ~root
** cat * | ./minishell <-- pas a executer mais a protéger
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
** <!<
** ls\ --> saut de ligne puis ls exec --> pas de saut de ligne dans l'histo
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
** >oui<<EOF
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
** cat <&\2
** echo test >folder/unfind_folder/file
** echo test >&0
** echo test > file | cat -->stdou prio sur pipe
** echo test | cat < file --> file écrase le pipe
** exit 2>file
** exit | test
** exit 1 2 --> too many arg et pas d'exit
** exit t --> exit mais pas msg "numeric arg required"
** / et /tmp --> "Is directory"
** auteur
**
*/

/*
** HARD TEST (et inutile)
** exit 2>file
*/

#endif

// Question :
//lors d'un output vers un tty quelle test faire pour check si tty ouvert
//Quelle logique dans "echo test 0>fichier" ?
//PATH=~/21sh --> foncitonnalité de TAB
