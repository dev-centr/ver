/** ver-gui — Qt front-end for the Ver library. */
module app_gui;

import core.runtime;
import core.stdcpp.new_;

import qt.config;
import qt.core.object;
import qt.core.string;
import qt.helpers;
import qt.widgets.application;
import qt.widgets.boxlayout;
import qt.widgets.label;
import qt.widgets.lineedit;
import qt.widgets.mainwindow;
import qt.widgets.plaintextedit;
import qt.widgets.pushbutton;
import qt.widgets.widget;

import ver;

class VerWindow : QMainWindow
{
	mixin(Q_OBJECT_D);

	this(QWidget parent = null)
	{
		super(parent);
		setWindowTitle("Ver");
		resize(720, 480);

		auto central = cpp_new!QWidget(this);
		auto layout = cpp_new!QVBoxLayout(central);

		auto title = cpp_new!QLabel(central);
		title.setText("Ver — universal VCS interface");
		layout.addWidget(title);

		auto prompt = cpp_new!QLabel(central);
		prompt.setText("Command (e.g. status, history, save hello)");
		layout.addWidget(prompt);

		input = cpp_new!QLineEdit(central);
		layout.addWidget(input);

		auto runBtn = cpp_new!QPushButton(central);
		runBtn.setText("Run");
		layout.addWidget(runBtn);

		output = cpp_new!QPlainTextEdit(central);
		output.setReadOnly(true);
		layout.addWidget(output);

		setCentralWidget(central);

		connect(runBtn.signal!"clicked", this.slot!"onRun");
		connect(input.signal!"returnPressed", this.slot!"onRun");
	}

private /+ slots +/:
	@QSlot final void onRun()
	{
		import std.array : split;
		import std.conv : to;
		import std.string : strip;

		auto line = input.text().to!string.strip;
		if (line.length == 0)
			return;
		auto parts = line.split();
		auto result = dispatch(parts[0], parts.length > 1 ? parts[1 .. $] : null);
		auto text = result.stdout;
		if (result.stderr.length)
			text ~= (text.length ? "\n" : "") ~ result.stderr;
		if (text.length == 0)
			text = "(exit " ~ result.exitCode.to!string ~ " via " ~ result.handlerName ~ ")";
		output.setPlainText(QString(text));
	}

private:
	QLineEdit input;
	QPlainTextEdit output;
}

int main()
{
	int argc = Runtime.cArgs.argc;
	char** argv = Runtime.cArgs.argv;
	scope app = new QApplication(argc, argv);

	auto window = cpp_new!VerWindow();
	window.show();
	return app.exec();
}
