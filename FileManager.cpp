#include "Stronghold.h"
void FileManager::saveGame(const char* fn, Economy& eco, Society& soc, Army& a, Resources& r, Leader& leader, EventHistory& history) {
    std::ofstream out(fn);
    if (!out) {
        cout << "Cannot open save file.\n";
        return;
    }
    out << eco.getGold() << " " << soc.totalPopulation() << " " << a.getSoldiers() << " " << a.getArchers() << " "
        << a.getCavalry() << " " << r.getFood() << " " << r.getWood() << " " << r.getStone() << " " << r.getIron() << " "
        << leader.getName() << " " << leader.getType() << " " << leader.getLegitimacy() << " " << leader.getPopularity() << "\n";
    history.save(out);
    out.close();
}
void FileManager::loadGame(const char* fn, Economy& eco, Society& soc, Army& a, Resources& r, Leader& leader, EventHistory& history) {
    std::ifstream in(fn);
    if (!in) {
        cout << "Cannot open load file.\n";
        return;
    }
    int gold, pop, soldiers, archers, cavalry, food, wood, stone, iron, leg, popu;
    char lname[MAX_NAME], ltype[MAX_NAME];
    in >> gold >> pop >> soldiers >> archers >> cavalry >> food >> wood >> stone >> iron >> lname >> ltype >> leg >> popu;
    eco = Economy(gold);
    a = Army(soldiers);
    a.recruitArchers(archers);
    a.recruitCavalry(cavalry);
    r = Resources(food, wood, stone, iron);
    leader = Leader(lname, ltype);
    history.load(in);
    in.close();
}
