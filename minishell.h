/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-amma <mel-amma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 09:39:21 by mel-amma          #+#    #+#             */
/*   Updated: 2022/03/04 13:26:35 by mel-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <term.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>

typedef struct s_arguments
{
	char				*arg;
	struct s_arguments	*next;
	struct s_arguments	*previous;

}				t_arguments;

typedef struct s_commandtable
{
	char					*command;
	t_arguments				*argument;//address of the first argument
	
	int						input_fd;// default stdin(0) is stdin or pipe //and fd for fd)
	int						output_fd;//default stdout(1)or pipe //fd for file fd
	int						self_input_fd;//will be -1 if its not filled, so if its -1 dont change the default one
	int						self_output_fd;

	int						is_a_command;
	int						is_a_file;
	int						is_a_fileR;
	int						is_a_fileA;
	int						is_heredoc; 
	char					*limiter;

	int						pipe;
	int						redirections;
	
	struct s_commandtable	*next;
	struct s_commandtable	*previous;


}				t_commandtable;

typedef struct s_line_processing_history
{
	struct sigaction	sa;
	int					got_command_yet;

/*1*/
	pid_t			process_id;
	char		**env_vars;
	int			exit_status;//> this shall take 0 on success and number of error when its error
	//put in waitpid 2nd arg(?)//
	t_commandtable	*command_table;//address of the first command_table
/*1*/
	int				*current_pipe;
	int				redisplayer;
	int				is_next_a_command;
	pid_t			parent_id;
	char			*entered_line;
	char			*trimmed_line;
	char			*dollar_processed;
	char			*expanded_line;
	struct termios	terminal_settings;
}				t_line_processing_history;

char			**ft_split_whitespaces(char *str);
t_arguments *match_with_directory(char *pattern);
void close_fds(t_commandtable *command_table);
int reached_a_redirection_or_end(char c);
int is_full_of_whitespaces(char *line);
int processline(t_line_processing_history *v_lines);
void print_ct(t_line_processing_history *v_lines);
int syntax_check(t_commandtable *commandtable);
int parse(t_line_processing_history *v_lines);
int expanded_line_2_command_table(t_line_processing_history *v_lines,
	int start, int redirection_number, char *line);
int	expandline(t_line_processing_history *v_lines);
int get_last_pipe(int *flag,char *line);
int pipe_redirection(char c, char after_c);
char *number_to_redirection(int c);
char *dollar_check(int *index,t_line_processing_history *v_lines, char *line);
char *expand_dollar(char *line, int var_start, int var_len, int type);
char *get_var(char *var);
char *delete_spaces(char *line, int start_eliminating);
int check_pipe_error(char *line, int i);
char *process_quotes(int *index,t_line_processing_history *v_lines, char *line);
char *process_single_quotes(char *line, int *end_quote, t_line_processing_history *v_lines);
char *process_double_quotes(char *line, int *end_quote, t_line_processing_history *v_lines);
void unclosed_quotes();
int split_into_table(char *line, t_commandtable *command_table);
t_arguments	*get_last_argument(t_arguments *argument);
void	initialize_argument_node(t_commandtable *command_table);
void	fill_limiter(char *line, t_commandtable *command_table);
void	fill_file(char *line, t_commandtable *command_table);
void	fill_command(char *line, t_commandtable *command_table);
void fill_arguments(t_commandtable *command_table, char *line, int *index);
char *clean_token(char *line, int *iteration_position);
char *delete_quote(char *token,int *start_quote_position);
int file_opener(int permission, char *file_name, int *read_fd, int *write_fd);
int make_herdoc_pipe(char *limiter);
void free_all(t_line_processing_history *v_lines);
int whitespaces(char c);
int initialize_command_table_node(t_line_processing_history *v_lines);
t_commandtable *get_last_command_line(t_commandtable *command_table);
void initialize_command_infos(t_commandtable *command_table);
char *check_redirection_error(char *line, int i, int redirection);
char *redirection_error(char *line, int i);
char *pipe_error(char *line, int i);
void free_command_table(t_commandtable *command_table);
void free_command_table_content(t_commandtable *command_table);
void free_arguments(t_arguments *argument);
void null_command_table_arguments(t_commandtable *command_table);
int handle_signals();
void exiting(int signum);
void get_parent_id(int signum, siginfo_t *siginfo, void *noUse);
void ctrl_backslash_handler(int signum);
void ctrl_c_handler(int signum, siginfo_t *siginfo, void *noUse);
int max(int a, int b);
int fill_command_type(t_commandtable *command_table, int type);
int fill_is_a_command(t_line_processing_history *v_lines,int redirection_number);
int handle_exit();
int initialize_v_lines(t_line_processing_history *v_lines, char **env);
int null_lines(t_line_processing_history *v_lines);
int free_lines(t_line_processing_history *v_lines);







//helpers
int	numbcount(int n);
char	*ft_itoa(int n);
char	*ft_strtrim(char const *s1, char const *set);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strdup(const char *s1);
int		ft_isalnum(int c);

//split

char			**ft_split_whitespaces(char *str);
#endif