phone(galaxy_a9) :- cores_num(8),
                    year(2018),
                    ram(8),
                    display_inches(6.5),
                    android(8).

phone(galaxy_a7) :- cores_num(8),
                    year(2018),
                    ram(6),
                    display_inches(6),
                    android(8),
                    card_slot(yes),
                    released(yes),
                    headphones_jack(yes).

phone(galaxy_note_10) :- cores_num(8),
                    year(2019),
                    stylus(yes).

phone(galaxy_s10) :- cores_num(8),
                    ram(8),
                    display_inches(6),
                    year(2019),
                    android(9),
                    card_slot(yes),
                    released(no),
                    headphones_jack(no).

phone(galaxy_s9) :- cores_num(6),
                    ram(6),
                    display_inches(5.5),
                    year(2018),
                    android(8),
                    card_slot(no),
                    released(yes),
                    headphones_jack(yes).

phone(galaxy_s8) :- cores_num(6),
                    ram(4),
                    display_inches(5.5),
                    year(2017),
                    android(7),
                    card_slot(yes),
                    released(yes),
                    headphones_jack(yes).

phone(galaxy_s7) :- cores_num(4),
                    ram(4),
                    display_inches(5),
                    year(2016),
                    android(6),
                    card_slot(yes),
                    released(yes),
                    headphones_jack(yes).

phone(galaxy_s6) :- cores_num(4),
                    ram(3),
                    display_inches(5),
                    year(2015),
                    android(6),
                    card_slot(no),
                    released(yes),
                    headphones_jack(yes).

phone(galaxy_s5) :- cores_num(4),
                    ram(2),
                    display_inches(5),
                    year(2014),
                    android(5),
                    card_slot(yes),
                    released(yes),
                    headphones_jack(yes).

phone(galaxy_s4) :- cores_num(4),
                    ram(2),
                    display_inches(5),
                    year(2013),
                    android(4),
                    card_slot(yes),
                    released(yes),
                    headphones_jack(yes).

phone(galaxy_s3) :- cores_num(2),
                    ram(1),
                    display_inches(4.5),
                    year(2012),
                    android(4),
                    card_slot(yes),
                    released(yes),
                    headphones_jack(yes).

phone(galaxy_s2) :- cores_num(2),
                    ram(1),
                    display_inches(4),
                    year(2011),
                    android(2),
                    card_slot(yes),
                    released(yes),
                    headphones_jack(yes).

cores_num(X) :- menuask(cores_num, X, [2, 4, 6, 8]).
ram(X) :- menuask(ram, X, [1, 2, 3, 4, 6, 8]).
display_inches(X) :- menuask(display_inches, X, [4, 4.5, 5, 5.5, 6, 6.5]).
year(X) :- menuask(year, X, [2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019]).
android(X) :- menuask(android, X, [2, 4, 5, 6, 7, 8, 9]).
card_slot(X) :- ask(card_slot, X).
released(X) :- ask(released, X).
headphones_jack(X) :- ask(headphones_jack, X).
stylus(X) :- ask(stylus, X).
cost(X) :- menuask(cost, X, [100, 200, 300, 400, 500, 600, 700, 800, 900]).

:- dynamic(known/3).
:- discontiguous menuask/3.
:- discontiguous ask/2.

% підійшло
ask(Attr, Val) :- known(yes, Attr, Val), !.
menuask(Attr, Val, _) :- known(yes, Attr, Val), !.

% не підійшло
ask(Attr, Val) :- known(_, Attr, Val), !, fail.
menuask(Attr, Val, _) :- known(_, Attr, Val), !, fail.
ask(Attr, Val) :- known(yes, Attr, V), V \== Val, !, fail.
menuask(Attr, Val, _) :- known(yes, Attr, V), V \== Val, !, fail.

ask(Attr, Val) :- write(Attr:Val), write('? '),
                           read(Ans),
                           asserta(known(Ans, Attr, Val)), Ans == yes.
menuask(Attr, Val, List) :-
                            write(Attr), write('? '), nl,
                            write(List), nl,
                            read(Ans),
                            check_val(Ans, Attr, Val, List),
                            asserta(known(yes, Attr, Ans)),
                            Ans == Val.
check_val(Ans, _, _, List) :- member(Ans, List), !.
check_val(Ans, Attr, Val, List) :-
                            write(Ans), write(' is not a known answer, please try again.'), nl,
                            menuask(Attr, Val, List).

system :- phone(Phone), write('The phone is '), write(Phone), nl.