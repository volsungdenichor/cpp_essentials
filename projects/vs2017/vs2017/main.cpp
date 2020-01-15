#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>
#include <atomic>

#include <cpp_essentials/geo/bounding_box.hpp>
#include <cpp_essentials/core/string_views.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/tuple.hpp>
#include <cpp_essentials/core/string_functors.hpp>
#include <string_view>
#include <cpp_essentials/meta/serialization_json.hpp>
#include <cpp_essentials/gx/bitmap.hpp>
#include <cpp_essentials/gx/morphological_operations.hpp>
#include <cpp_essentials/gx/kernels.hpp>
#include <cpp_essentials/gx/lookup_table.hpp>
#include <cpp_essentials/gx/drawing_context.hpp>
#include <cpp_essentials/gx/filters.hpp>
#include <cpp_essentials/gx/colors.hpp>
#include <complex>
#include <cpp_essentials/geo/intersection.hpp>
#include <cpp_essentials/geo/projection.hpp>
#include <cpp_essentials/geo/distance.hpp>
#include <cpp_essentials/geo/circle.hpp>
#include <cpp_essentials/geo/coordinates_conversion.hpp>
#include <cpp_essentials/core/format.hpp>
#include <cpp_essentials/core/map_utils.hpp>
#include <cpp_essentials/core/expected.hpp>
#include <cpp_essentials/graphs/algorithm.hpp>
#include <cpp_essentials/graphs/tree.hpp>
#include <cpp_essentials/gx/filters.hpp>
#include <cpp_essentials/core/optional_helpers.hpp>
#include <cpp_essentials/core/string_functors.hpp>
#include <cpp_essentials/core/any_range.hpp>

#include <cpp_essentials/kine/animation.hpp>
#include <cpp_essentials/geo/orientation.hpp>
#include <cpp_essentials/geo/clamp.hpp>
#include <cpp_essentials/core/generator.hpp>
#include <cpp_essentials/geo/dcel.hpp>
#include <cpp_essentials/core/program_args.hpp>
#include <cpp_essentials/core/match.hpp>

#include <cpp_essentials/arrays/array.hpp>
#include <cpp_essentials/arrays/transformations.hpp>
#include <cpp_essentials/geo/triangulation.hpp>
#include <cpp_essentials/core/debug_utils.hpp>
#include <cpp_essentials/core/channel.hpp>
#include <cpp_essentials/core/task_queue.hpp>
#include <cpp_essentials/core/event_aggregator.hpp>

#include <cpp_essentials/carto/coords_array.hpp>
#include <cpp_essentials/math/binomial.hpp>

#include <cpp_essentials/fmt/format.hpp>

#include <cpp_essentials/math/regression.hpp>
#include <cpp_essentials/chrono/calendars.hpp>

using namespace cpp_essentials;

enum class Player
{
    none,
    cross,
    circle
};

std::ostream& operator <<(std::ostream& os, Player item)
{
    switch (item)
    {
        case Player::none:
            return os << '.';
        case Player::cross:
            return os << 'X';
        case Player::circle:
            return os << 'O';
    }
    return os;
}

using Board = arrays::array_2d<Player>;

bool is_winner(Board::const_view_type board, Player player)
{
    const auto is_winning_slice = [=](Board::const_slice_type slice)
    {
        return core::all_of(slice, core::equal_to(player));
    };

    return core::any_of(board.row_range(), is_winning_slice)
        || core::any_of(board.column_range(), is_winning_slice)
        || is_winning_slice(arrays::diagonal(board, 0))
        || is_winning_slice(arrays::diagonal(board, 1));
}

static const auto is_free = core::equal_to(Player::none);

bool game_over(Board::const_view_type board)
{
    return core::none_of(board, is_free);
}

core::optional<Player> get_winner(Board::const_view_type board)
{
    for (Player player : { Player::circle, Player::cross})
    {
        if (is_winner(board, player))
        {
            return player;
        }
    }
    return core::none;
}

Player get_opponent(Player player)
{
    switch (player)
    {
        case Player::cross: return Player::circle;
        case Player::circle: return Player::cross;
        default: return player;
    }
}

using Move = Board::location_type;


std::tuple<core::optional<Move>, int> next_move(Board::view_type board, Player player, Player current_player, int depth)
{
    if (depth == 0 || game_over(board))
    {
        return { core::none, 0 };
    }
    else if (is_winner(board, current_player))
    {
        return { core::none, +10 + depth };
    }
    else if (is_winner(board, get_opponent(current_player)))
    {
        return { core::none, -10 - depth };
    }

    auto best_move = core::optional<Move>{};
    auto best_score = core::optional<int>{};

    const auto is_best = player == current_player
        ? core::comparison<int>{ core::greater_equal }
        : core::comparison<int>{ core::less_equal };


    for (auto it : core::views::iterate(board))
    {
        if (is_free(*it))
        {

            *it = player;
            const auto[move, score] = next_move(board, get_opponent(player), current_player, depth - 1);
            if (!best_score || is_best(score, *best_score))
            {
                best_score = score;
                best_move = it.location();
            }
            *it = Player::none;
        }
    }

    return { best_move, best_score.value_or(0) };
}

core::optional<Move> next_move(Board::const_view_type board, Player player)
{
    Board temp_board{ board };
    const auto[next, score] = next_move(temp_board, player, player, core::count_if(board, is_free));
    (void)score;
    return next;
}

void print(std::ostream& os, Board::const_view_type board, std::string_view prefix = {})
{
    for (auto row : board.row_range())
    {
        os << prefix << "|" << core::delimit(row, " ") << "|" << std::endl;
    }
}

void run()
{
    auto board = Board{ { 3, 3 } };

    auto player = Player::cross;
    while (true)
    {
        if (auto next = next_move(board, player))
        {
            board[*next] = player;
            print(std::cout, board, player == Player::cross ? "" : "     ");

            player = get_opponent(player);
        }
        
        if (auto winner = get_winner(board))
        {
            std::cout << "Winner is " << *winner << std::endl;
            break;
        }
        else if (game_over(board))
        {
            std::cout << "Game over" << std::endl;
            break;                
        }
    }
}

int main(int argc, char** argv)
{
    const auto start = std::chrono::high_resolution_clock::now();
    try
    {
        run();
    }
    catch (std::exception& ex)
    {
        std::cerr << "exception: " << ex.what() << std::endl;
    }
    const auto stop = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Elapsed " << elapsed << "ms" << std::endl;
    return 0;
}
