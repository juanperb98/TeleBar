#include <telebar/handler/serverHandler/serverGameHandler/serverGetBoardHandler.hpp>

std::string serverGetBoardHandler(ORM &orm, User user, std::string payload) {
    if (payload == "pipo"){
        std::string dogo = "                            ;\\ \n"
                           "                            |' \\ \n"
                           "         _                  ; : ; \n"
                           "        / `-.              /: : | \n"
                           "       |  ,-.`-.          ,': : | \n"
                           "       \\  :  `. `.       ,'-. : | \n"
                           "        \\ ;    ;  `-.__,'    `-.| \n"
                           "         \\ ;   ;  :::  ,::'`:.  `. \n"
                           "          \\ `-. :  `    :.    `.  \\ \n"
                           "           \\   \\    ,   ;   ,:    (\\ \n"
                           "            \\   :., :.    ,'o)): ` `-. \n"
                           "           ,/,' ;' ,::\"'`.`---'   `.  `-._ \n"
                           "         ,/  :  ; '\"      `;'          ,--`. \n"
                           "        ;/   :; ;             ,:'     (   ,:) \n"
                           "          ,.,:.    ; ,:.,  ,-._ `.     \\\"\"'/ \n"
                           "          '::'     `:'`  ,'(  \\`._____.-'\"' \n"
                           "             ;,   ;  `.  `. `._`-.  \\\\ \n"
                           "             ;:.  ;:       `-._`-.\\  \\`. \n"
                           "              '`:. :        |' `. `\\  ) \\ \n"
                           "      -hrr-      ` ;:       |    `--\\__,' \n"
                           "                   '`      ,' \n"
                           "                        ,-' \n";
        return std::string("EASTEREGG,\n") + dogo;

    }

    Game game = orm.getById<Game>(user.getGameId());

    if (game.getId() == -1)
        return "ERROR,game not found, try joining a game";
    else
        return std::string("OK") + game.serializeForPlayer(user.getId());;
}


//     __
//  __( o)>
//  \ <_ ) r90