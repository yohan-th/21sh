/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   shell_process.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ythollet <ythollet@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/06/28 20:39:43 by ythollet     #+#   ##    ##    #+#       */
/*   Updated: 2018/06/28 20:39:57 by ythollet    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

/*
** ATTENTION shell->str peut etre vide apres un heredoc
*/

#include "../../Include/shell.h"


BOOL	shell_runcmd(t_cmd *cmd, t_shell *shell)
{
	execve(cmd->exec, cmd->args, shell->envp);
	return (1);
}

int		shell_process(t_cmd **cmd, t_shell *shell)
{
	t_cmd	*link;
	//int		father;
	//int		pfd[2];

	shell_prepare(*cmd, shell);
	read_lexing(*cmd);
	link = *cmd;
	/*
	while ((link = link->next_cmd))
	{
		pipe(pfd);
		father = vfork();
		if (!father)
		{
			if (link->args && link->exec)
			{
				close(pfd[1]);  //close the unused write side
				dup2(pfd[0], 0); // connect the read side with stdin
				close(pfd[0]);  //close the read side
				shell_runcmd(link, shell);
			}
			else
			{
				printf("-<|exit|\n");
				exit(0);
			}
		}
		else
		{
			close(pfd[0]); // close the unused read side
			dup2(pfd[1], 1); // connect the write side with stdout
			close(pfd[1]); // close the write side
			//if (link->std_in)
			//	shell_prcs_send_stdin()
			wait(NULL);
		}

		//shell_quotesub(*cmd);
		//shell_envpsub(&arg, envp, quote);
	}
	 */
	if (shell->str && ft_strcmp(shell->str, "exit") == 0)
	{
		clean_shell(&shell);
		clean_cmd(cmd);
		exit(1);
	}
	clean_cmd(cmd);
	printf("-<process|%p|\n", *cmd);
	ft_strdel(&shell->str);
	return (1);
	//clean_shell(&shell);
}



void	read_lexing(t_cmd *cmd)
{
	t_stdout	*read;
	int 		i;

	while ((cmd = cmd->next_cmd))
	{
		dprintf(2, "Read exec : %s\n", cmd->exec);
		i = 0;
		dprintf(2, "Read array : ");
		while (cmd->args[i])
		{
			dprintf(2, "arg[%i]=<%s> ", i, cmd->args[i]);
			i++;
		}
		dprintf(2, "\nRead stdout : ");
		read = cmd->std_out;
		while (read != NULL)
		{
			dprintf(2, "from %d to <%s> append=%d - ", read->from, read->to, read->append);
			read = read->next;
		}
		dprintf(2, "\nRead stdin : ");
		i = 0;
		while (cmd->std_in && (cmd->std_in)[i] != NULL)
		{
			if ((int)(cmd->std_in)[i] <= -1 && (int)(cmd->std_in)[i] >= -3)
				dprintf(2, "<%d> -", (int)(cmd->std_in)[i++]);
			else
				dprintf(2, "<%s> - ", (cmd->std_in)[i++]);
		}
		dprintf(2, "\nRead heredoc : ");
		i = 0;
		while (cmd->hrdc && (cmd->hrdc)[i] != NULL)
		{
			if ((int)(cmd->hrdc)[i] <= -1 && (int)(cmd->hrdc)[i] >= -3)
				dprintf(2, "<%d> -", (int)(cmd->hrdc)[i++]);
			else
				dprintf(2, "<%s> - ", (cmd->hrdc)[i++]);
		}
		if ((int)cmd->hrdc_stdin == -1)
			dprintf(2, "\nRead heredoc stdin : <%d>\n", (int)cmd->hrdc_stdin);
		else
			dprintf(2, "\nRead heredoc stdin : <%s>\n", cmd->hrdc_stdin);
		dprintf(2, "Et sep %d\n", cmd->sep);
		dprintf(2, "-------------\n");
	}
}
