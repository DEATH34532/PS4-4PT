//
// Created by Víctor Jiménez Rugama on 12/24/21.
//
#ifndef CYDI4_REPOSITORYVIEW_H
#define CYDI4_REPOSITORYVIEW_H
#ifndef GRAPHICS_USES_FONT
#define GRAPHICS_USES_FONT 1
#endif

#include "subView.h"
#include "../repository/repository.h"
#include "../../_common/graphics.h"
#include "../../include/view/keyboardInput.h"
#include "../utils/AnimatedPNG.h"

#include <vector>
class repositoryView : public subView {
    enum SelectedOption {
        OPEN, UPDATE, DELETE
    };
    struct repoRectangle {
        int x;
        int y;
        int width;
        int height;
    };
private:
    PNG * deleteIcon;
    PNG * deleteIconSelected;
    int deleteIconX;
    int deleteRepo(const char * id);

    PNG * openIcon;
    PNG * openIconSelected;
    int openIconX;

    AnimatedPNG * updateIcon;
    PNG * updateIconSelected;
    int updateIconX;

    int repoIconX;
    const static int reposPerPage = 5;
    int currPage;
    int selected;
    int rectangleBaseHeight;
    int rectangleDivisorHeight;
    Scene2D * mainScene;
    repository * currRepos[reposPerPage];
    void updateRepositories();
    std::shared_ptr<std::vector<repository*>> repositoryList;
    repoRectangle repoRectangles[reposPerPage];
    subView * child;
    Color bgColor{};
    Color textColor{};
    Color selectedColor{};
    FT_Face fontLarge{};
    FT_Face fontMedium{};
    FT_Face fontSmall{};

    keyboardInput * keyboardInput;


    SelectedOption selectedOption;

    bool isOnKeyboard;
    const int frameWidth = 0;
    const int frameHeight = 0;
    const int viewWidth = 0;
    const int viewHeight = 0;
    void fillPage();
    void hasEntered();
public:
    static repositoryView * mainRepositoryView;
    repositoryView(Scene2D* mainScene, FT_Face fontLarge, FT_Face fontMedium, FT_Face fontSmall, int frameWidth, int frameHeight, bool isFirstRun);
    void updateView();
    void pressX();
    void pressCircle();
    void pressTriangle();
    void pressSquare();
    void arrowUp();
    void arrowDown();
    void arrowRight();
    void arrowLeft();
    void deleteChild();
    subView * getParent();
    subView * getChild();

    std::shared_ptr<std::vector<repository*>> getRepositoryList();

    bool isActive();
    ~repositoryView();
};
#endif //CYDI4_REPOSITORYVIEW_H
