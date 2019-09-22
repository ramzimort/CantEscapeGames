
class Logger
{
public:
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
	bool                AutoScroll;     // Keep scrolling if already at the bottom

	Logger();
	void Clear();
	void AddLog(const char* fmt, ...) IM_FMTARGS(2);
	void Draw(const char* title, bool* p_open = NULL);
};