module ver.detect;

import std.file : exists, isDir;
import std.path : buildPath;
import ver.types;

/** Walk upward from `startDir` looking for a VCS root. */
string findVcsRoot(string startDir)
{
	import std.path : absolutePath, dirName;

	auto dir = absolutePath(startDir);
	while (true)
	{
		if (exists(buildPath(dir, ".git")))
			return dir;
		if (exists(buildPath(dir, ".svn")) && isDir(buildPath(dir, ".svn")))
			return dir;
		if (exists(buildPath(dir, ".hg")) && isDir(buildPath(dir, ".hg")))
			return dir;
		auto parent = dirName(dir);
		if (parent == dir)
			return null;
		dir = parent;
	}
}

/** Detect VCS kind for `dir` (or its nearest ancestor root). */
VcsKind detectVcs(string dir = ".")
{
	auto root = findVcsRoot(dir);
	if (root is null)
		return VcsKind.none;
	if (exists(buildPath(root, ".git")))
		return VcsKind.git;
	if (exists(buildPath(root, ".svn")))
		return VcsKind.svn;
	if (exists(buildPath(root, ".hg")))
		return VcsKind.hg;
	return VcsKind.unknown;
}

string vcsKindName(VcsKind kind)
{
	final switch (kind)
	{
	case VcsKind.none:
		return "none";
	case VcsKind.git:
		return "git";
	case VcsKind.svn:
		return "svn";
	case VcsKind.hg:
		return "hg";
	case VcsKind.unknown:
		return "unknown";
	}
}
