#include "uwhd/model/GameModel.h"
#include "uwhd/canvas/Canvas.h"
#include "uwhd/canvas/PPMCanvasViewer.h"
#include "uwhd/display/GameDisplay.h"

#include <fstream>
#include <sstream>
#include <iostream>

void printUsage(int argc, const char *argv[]) {
  std::cerr << "Usage: " << argv[0] << " <inflie>" << std::endl;
  exit(1);
}

bool startsWith(const std::string &Str, const std::string &Prefix) {
  return Str.find(Prefix) == 0;
}

struct ActionResult {
  ActionResult() : M(), Dump(false) {}

  GameModel M;
  bool Dump;
  unsigned Version;
};

void parseComment(const std::string &Line, ActionResult &AR) {
  std::stringstream SS(Line);

  if (startsWith(Line, "# SET-VERSION:")) {
    SS.seekg(sizeof("# SET-VERSION:"));
    unsigned Version;
    SS >> Version;
    AR.Version = Version;
    return;
  }

  if (startsWith(Line, "# SET-BLACK:")) {
    SS.seekg(sizeof("# SET-BLACK:"));
    unsigned Score;
    SS >> Score;
    AR.M.BlackScore = Score;
    return;
  }

  if (startsWith(Line, "# SET-WHITE:")) {
    SS.seekg(sizeof("# SET-WHITE:"));
    unsigned Score;
    SS >> Score;
    AR.M.WhiteScore = Score;
    return;
  }

  if (startsWith(Line, "# SET-TIME:")) {
    SS.seekg(sizeof("# SET-TIME:"));
    unsigned Time;
    SS >> Time;
    AR.M.GameClockSecs = Time;
    return;
  }

  if (startsWith(Line, "# SET-DUMP")) {
    AR.Dump = true;
    return;
  }

  struct Directive {
    const char *Str;
    GameModel::GameState State;
  } const Directives[] = {
    { "# SET-STATE: WallClock",    GameModel::WallClock },
    { "# SET-STATE: FirstHalf",    GameModel::FirstHalf },
    { "# SET-STATE: SecondHalf",   GameModel::SecondHalf },
    { "# SET-STATE: RefTimeOut",   GameModel::RefTimeOut },
    { "# SET-STATE: WhiteTimeOut", GameModel::WhiteTimeOut },
    { "# SET-STATE: BlackTimeOut", GameModel::BlackTimeOut },
    { "# SET-STATE: GameOver",     GameModel::GameOver },
  };

  for (const Directive &D : Directives) {
    if (startsWith(Line, D.Str)) {
      AR.M.State = D.State;
      return;
    }
  }

  if (startsWith(Line, "# SET")) {
    std::cerr << "error: unknown SET command: '" << Line << "'\n";
    exit(1);
  }
}

int main(int argc, const char *argv[]) {
  if (argc != 2)
    printUsage(argc, argv);

  std::ifstream IF(argv[1]);
  std::stringstream PPMStr;
  ActionResult AR;
  for (std::string Line; std::getline(IF, Line); ) {
    if (Line[0] == '#')
      parseComment(Line, AR);
    else
      PPMStr << Line << "\n";
  }

  std::unique_ptr<UWHDCanvas> FromDirectives(UWHDCanvas::create(32 * 3, 32));
  if (!FromDirectives) {
    std::cerr << "error: could not allocate a canvas\n";
    return 1;
  }

  renderGameDisplay(AR.Version, AR.M, FromDirectives.get());

  if (AR.Dump) {
    std::cout << asPPMString(FromDirectives.get()) << std::endl;
    return 0;
  }

  std::unique_ptr<UWHDCanvas> FromText(createCanvasFromPPMString(PPMStr.str()));
  if (!FromText) {
    std::cerr << "error: could not parse PPM data\n";
    return 1;
  }

  if (FromText->w != 96) {
    std::cerr << "error: incorrect width\n"
              << "  expected: 96\n"
              << "  got:      " << FromText->w << "\n";
    return 1;
  }

  if (FromText->h != 32) {
    std::cerr << "error: incorrect height\n"
              << "  expected: 32\n"
              << "  got:      " << FromText->h << "\n";
    return 1;
  }

  FromText->forEach([&](unsigned X, unsigned Y) {
    if (FromText->at(X, Y) != FromDirectives->at(X, Y)) {
      std::cerr << "error: mismatch at (" << X << "," << Y << ")\n"
                << "  expected: " << FromText->at(X, Y) << "\n"
                << "  got:      " << FromDirectives->at(X, Y) << "\n";
      exit(1);
    }
  });

  return 0;
}

