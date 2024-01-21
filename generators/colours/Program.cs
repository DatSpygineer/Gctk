using System.Drawing;
using System.Text;

var header = new StringBuilder();
var src = new StringBuilder();
var knownColours = new StringBuilder("enum class KnownColor {\n");
var byNameFnBody = new StringBuilder("Color Color::FromName(const std::string& name) {\n");
var byEnumFnBody = new StringBuilder("Color Color::FromKnownColor(const KnownColor color) {\n\tswitch (color) {\n");

foreach (KnownColor colorName in Enum.GetValues(typeof(KnownColor))) {
	header.AppendLine($"static const Color {colorName};");
	var color = Color.FromKnownColor(colorName);
	src.AppendLine($"const Color Color::{colorName} = {{ {(float)color.R / 255.0f}, {(float)color.G / 255.0f}, {(float)color.B / 255.0f}, {(float)color.A / 255.0f} }};");
	byNameFnBody.AppendLine($"\tif (name == \"{colorName}\") return Color::{colorName};");
	byEnumFnBody.AppendLine($"\t\tcase KnownColor::{colorName}: return Color::{colorName};");
	knownColours.AppendLine($"\t{colorName},");
}

byNameFnBody.AppendLine("\treturn { 0, 0, 0, 1 };\n}");
byEnumFnBody.AppendLine("\t\tdefault: return { 0, 0, 0, 1 };\n\t}\n}");
byEnumFnBody.AppendLine("Color Color::FromKnownColour(const KnownColor name) { return FromKnownColor(name); }");
knownColours.AppendLine("};\nusing KnownColour = KnownColor;");

header.AppendLine();
header.AppendLine(knownColours.ToString());

header.AppendLine("static Color FromName(const std::string& name);");
header.AppendLine("static Color FromKnownColor(const KnownColor color);");
header.AppendLine("static Color FromKnownColour(const KnownColor name);");

src.AppendLine();
src.AppendLine(byNameFnBody.ToString());
src.AppendLine(byEnumFnBody.ToString());

var baseDir = Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "..", "include", "generated"));
using var outHeader = File.OpenWrite(Path.Combine(baseDir, "gctk_colours.hpp"));
using var outHeaderWriter = new StreamWriter(outHeader);
outHeaderWriter.Write(header.ToString());

using var outSrc = File.OpenWrite(Path.Combine(baseDir, "gctk_colours_impl.hpp"));
using var outSrcWriter = new StreamWriter(outSrc);
outSrcWriter.Write(src.ToString());