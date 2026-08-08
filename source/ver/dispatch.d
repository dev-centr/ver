module ver.dispatch;

import ver.detect;
import ver.handlers;
import ver.types;

/** Dispatch a Ver command using the detected VCS under `workDir`. */
CommandResult dispatch(string command, string[] args, string workDir = ".")
{
	if (command == "help" || command == "--help")
		return CommandResult(0, helpText(), "", "ver");

	if (command == "version" || command == "--version")
	{
		import ver : verVersion;
		return CommandResult(0, "Ver " ~ verVersion ~ "\n", "", "ver");
	}

	if (command == "init")
	{
		auto kind = detectVcs(workDir);
		if (kind == VcsKind.none || kind == VcsKind.git)
			return gitHandle("init", args, workDir);
		if (kind == VcsKind.hg)
			return hgHandle("init", args, workDir);
		return CommandResult(2, "", "cannot init: existing non-git/hg VCS", "ver");
	}

	auto kind = detectVcs(workDir);
	final switch (kind)
	{
	case VcsKind.git:
		return gitHandle(command, args, workDir);
	case VcsKind.svn:
		return svnHandle(command, args, workDir);
	case VcsKind.hg:
		return hgHandle(command, args, workDir);
	case VcsKind.none:
		return CommandResult(1, "", "No version control system detected. Use 'ver init'.", "ver");
	case VcsKind.unknown:
		return CommandResult(1, "", "Unknown VCS detected.", "ver");
	}
}
