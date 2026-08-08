module ver.types;

/** Identifies which VCS backend is active for a working tree. */
enum VcsKind
{
	none,
	git,
	svn,
	hg,
	unknown,
}

/** Result of running a mapped Ver command. */
struct CommandResult
{
	int exitCode;
	string stdout;
	string stderr;
	string handlerName;
}
