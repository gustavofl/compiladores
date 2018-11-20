/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    PROGRAMA = 258,
    TIPO = 259,
    VAZIO = 260,
    INT = 261,
    REAL = 262,
    NUM_INT = 263,
    NUM_REAL = 264,
    ID = 265,
    EXPR = 266,
    ATTR = 267,
    OU = 268,
    E = 269,
    NAO = 270,
    SE = 271,
    SENAO = 272,
    ENQUANTO = 273,
    FUNCAO = 274,
    ESCREVA = 275,
    LEIA = 276,
    CADEIA = 277,
    PULALINHA = 278,
    MAIOR_IGUAL = 279
  };
#endif
/* Tokens.  */
#define PROGRAMA 258
#define TIPO 259
#define VAZIO 260
#define INT 261
#define REAL 262
#define NUM_INT 263
#define NUM_REAL 264
#define ID 265
#define EXPR 266
#define ATTR 267
#define OU 268
#define E 269
#define NAO 270
#define SE 271
#define SENAO 272
#define ENQUANTO 273
#define FUNCAO 274
#define ESCREVA 275
#define LEIA 276
#define CADEIA 277
#define PULALINHA 278
#define MAIOR_IGUAL 279

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
