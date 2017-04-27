# ~/.profile: executed by the command interpreter for login shells.
# This file is not read by bash(1), if ~/.bash_profile or ~/.bash_login
# exists.
# see /usr/share/doc/bash/examples/startup-files for examples.
# the files are located in the bash-doc package.

# the default umask is set in /etc/profile; for setting the umask
# for ssh logins, install and configure the libpam-umask package.
#umask 022
alias errcho=">&2 echo"
# if running bash
if [ -n "$BASH_VERSION" ]; then
    # include .bashrc if it exists
    if [ -f "$HOME/.bashrc" ]; then
	. "$HOME/.bashrc"
    fi
fi
export set AWS_ACCESS_KEY_ID="AKIAJHZDBGZJXSJTDHCA"
export set AWS_SECRET_ACCESS_KEY="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"
export set AWS_DEFAULT_REGION="us-east-1"
export set AWS_DEFAULT_PROFILE="default"

# set PATH so it includes user's private bin if it exists
if [ -d "$HOME/bin" ] ; then
    PATH="$HOME/bin:$PATH"
fi
