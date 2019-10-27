#include <telebar/handler/serverHandler/serverGameHandler/serverGetBoardHandler.hpp>
#include <telebar/entity/UserNotification.hpp>

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

    /*if (game.hasEnded()) {

        User gameUser;
        for (auto& player : game.getPlayers()) {
            gameUser = orm.getById<User>(player.userId);
            gameUser.setGameId(-1);
            orm.update(gameUser);
        }
        for (auto& player : game.getPlayers()) {
            orm.save(UserNotification(player.userId, game.getId(), game.getTableName(), GAME_EVENT_THE_GAME_HAS_ENDED));
        }

    }*/

    else if (game.hasEnded())
        return std::string("OK,") + game.serialize();
    else
        return std::string("OK,") + game.serializeForPlayer(user.getId());
}


//     __
//  __( o)>
//  \ <_ ) r90