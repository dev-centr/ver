module ver.handlers;

import std.array : join;
import std.process : Config, execute;
import std.string : strip;
import ver.types;

private CommandResult runTool(string name, string[] args, string workDir)
{
	auto result = execute([name] ~ args, null, Config.none, size_t.max, workDir);
	return CommandResult(result.status, result.output, "", name);
}

/** Git-backed handler for the Ver command vocabulary. */
CommandResult gitHandle(string command, string[] args, string workDir)
{
	switch (command)
	{
	case "status":
		return runTool("git", ["status", "--short", "--branch"], workDir);
	case "history":
		return runTool("git", ["log", "--oneline", "-n", "20"], workDir);
	case "save":
		if (args.length == 0)
			return CommandResult(2, "", "save requires a message", "git");
		auto msg = args.join(" ");
		auto add = runTool("git", ["add", "-A"], workDir);
		if (add.exitCode != 0)
			return add;
		return runTool("git", ["commit", "-m", msg], workDir);
	case "stage":
		if (args.length == 0)
			return runTool("git", ["add", "-A"], workDir);
		return runTool("git", ["add"] ~ args, workDir);
	case "sync":
		auto pull = runTool("git", ["pull", "--rebase", "--autostash"], workDir);
		if (pull.exitCode != 0)
			return pull;
		return runTool("git", ["push"], workDir);
	case "branch":
		if (args.length == 0)
			return runTool("git", ["branch", "-vv"], workDir);
		if (args[0] == "switch" && args.length > 1)
			return runTool("git", ["switch", args[1]], workDir);
		return runTool("git", ["branch"] ~ args, workDir);
	case "init":
		return runTool("git", ["init"], workDir);
	case "merge":
		if (args.length == 0)
			return CommandResult(2, "", "merge requires a branch name", "git");
		return runTool("git", ["merge"] ~ args, workDir);
	default:
		return CommandResult(2, "", "unsupported command for git: " ~ command, "git");
	}
}

CommandResult svnHandle(string command, string[] args, string workDir)
{
	switch (command)
	{
	case "status":
		return runTool("svn", ["status"], workDir);
	case "history":
		return runTool("svn", ["log", "-l", "20"], workDir);
	case "save":
		if (args.length == 0)
			return CommandResult(2, "", "save requires a message", "svn");
		return runTool("svn", ["commit", "-m", args.join(" ")], workDir);
	case "stage":
		return CommandResult(0, "svn has no separate stage step", "", "svn");
	case "sync":
		return runTool("svn", ["update"], workDir);
	case "init":
		return CommandResult(2, "", "use svnadmin create / svn checkout for svn repos", "svn");
	default:
		return CommandResult(2, "", "unsupported command for svn: " ~ command, "svn");
	}
}

CommandResult hgHandle(string command, string[] args, string workDir)
{
	switch (command)
	{
	case "status":
		return runTool("hg", ["status"], workDir);
	case "history":
		return runTool("hg", ["log", "-l", "20"], workDir);
	case "save":
		if (args.length == 0)
			return CommandResult(2, "", "save requires a message", "hg");
		return runTool("hg", ["commit", "-m", args.join(" ")], workDir);
	case "stage":
		return CommandResult(0, "hg stages at commit time", "", "hg");
	case "sync":
		auto pull = runTool("hg", ["pull", "-u"], workDir);
		if (pull.exitCode != 0)
			return pull;
		return runTool("hg", ["push"], workDir);
	case "init":
		return runTool("hg", ["init"], workDir);
	default:
		return CommandResult(2, "", "unsupported command for hg: " ~ command, "hg");
	}
}

string helpText()
{
	return q"EOS
Ver - Universal Version Control Interface

Usage: ver <command> [options]

Commands:
  init              Initialize a repository (git/hg)
  save <msg>        Commit / save changes with a message
  stage [paths...]  Stage files (git) or no-op elsewhere
  status            Show working-tree status
  history           Show recent history
  sync              Pull then push (or svn update)
  branch [...]      List or manage branches
  merge <branch>    Merge a branch (git)
  help              Show this help
  version           Show version

Provider-specific tooling is selected from the nearest VCS root.
EOS".strip;
}
