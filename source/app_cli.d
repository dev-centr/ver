module app_cli;

import std.stdio : stderr, write, writeln;
import ver;

int main(string[] args)
{
	if (args.length < 2)
	{
		write(helpText());
		return 0;
	}

	auto result = dispatch(args[1], args[2 .. $]);
	if (result.stdout.length)
		write(result.stdout);
	if (result.stderr.length)
		stderr.write(result.stderr);
	if (result.exitCode != 0 && result.stderr.length == 0 && result.stdout.length == 0)
		stderr.writeln(result.handlerName, ": command failed with code ", result.exitCode);
	return result.exitCode;
}
