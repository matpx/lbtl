/**
 * @file addons/expr.h
 * @brief Flecs expression parser addon.
 *
 * Parse expression strings into component values. The notation is similar to
 * JSON but with a smaller footprint, native support for (large) integer types,
 * character types, enumerations, bitmasks and entity identifiers.
 * 
 * Examples:
 * 
 * Member names:
 *   {x: 10, y: 20}
 * 
 * No member names (uses member ordering):
 *   {10, 20}
 * 
 * Enum values:
 *   {color: Red}
 * 
 * Bitmask values:
 *   {toppings: Lettuce|Tomato}
 * 
 * Collections:
 *   {points: [10, 20, 30]}
 * 
 * Nested objects:
 *   {start: {x: 10, y: 20}, stop: {x: 30, y: 40}}
 * 
 */

#ifdef FLECS_EXPR

#ifndef FLECS_META
#define FLECS_META
#endif

#ifndef FLECS_PARSER
#define FLECS_PARSER
#endif

#ifndef FLECS_EXPR_H
#define FLECS_EXPR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup c_addons_expr Expr
 * @brief Serialize/deserialize values to string.
 * 
 * \ingroup c_addons
 * @{
 */

/** Write an escaped character.
 * Write a character to an output string, insert escape character if necessary.
 * 
 * @param out The string to write the character to.
 * @param in The input character.
 * @param delimiter The delimiiter used (for example '"')
 * @return Pointer to the character after the last one written.
 */
FLECS_API
char* ecs_chresc(
    char *out, 
    char in, 
    char delimiter);

/** Parse an escaped character.
 * Parse a character with a potential escape sequence.
 * 
 * @param in Pointer to character in input string.
 * @param out Output string.
 * @return Pointer to the character after the last one read.
 */ 
const char* ecs_chrparse(
    const char *in, 
    char *out);

/** Write an escaped string.
 * Write an input string to an output string, escape characters where necessary.
 * To determine the size of the output string, call the operation with a NULL
 * argument for 'out', and use the returned size to allocate a string that is
 * large enough.
 * 
 * @param out Pointer to output string (msut be).
 * @param size Maximum number of characters written to output.
 * @param delimiter The delimiter used (for example '"').
 * @param in The input string.
 * @return The number of characters that (would) have been written.
 */
FLECS_API
ecs_size_t ecs_stresc(
    char *out, 
    ecs_size_t size, 
    char delimiter, 
    const char *in);

/** Return escaped string.
 * Return escaped version of input string. Same as ecs_stresc, but returns an
 * allocated string of the right size.
 * 
 * @param delimiter The delimiter used (for example '"').
 * @param in The input string.
 * @return Escaped string.
 */
FLECS_API
char* ecs_astresc(
    char delimiter, 
    const char *in);

/** Storage for parser variables. Variables make it possible to parameterize
 * expression strings, and are referenced with the $ operator (e.g. $var). */
typedef struct ecs_expr_var_t {
    char *name;
    ecs_value_t value;
    bool owned; /* Set to false if ecs_vars_t should not take ownership of var */
} ecs_expr_var_t;

typedef struct ecs_expr_var_scope_t {
    ecs_hashmap_t var_index;
    ecs_vec_t vars;
    struct ecs_expr_var_scope_t *parent;
} ecs_expr_var_scope_t;

typedef struct ecs_vars_t {
    ecs_world_t *world;
    ecs_expr_var_scope_t root;
    ecs_expr_var_scope_t *cur;
} ecs_vars_t;

/** Init variable storage */
FLECS_API
void ecs_vars_init(
    ecs_world_t *world,
    ecs_vars_t *vars);

/** Cleanup variable storage */
FLECS_API
void ecs_vars_fini(
    ecs_vars_t *vars);

/** Push variable scope */
FLECS_API
void ecs_vars_push(
    ecs_vars_t *vars);

/** Pop variable scope */
FLECS_API
int ecs_vars_pop(
    ecs_vars_t *vars);

/** Declare variable in current scope */
FLECS_API
ecs_expr_var_t* ecs_vars_declare(
    ecs_vars_t *vars,
    const char *name,
    ecs_entity_t type);

/** Declare variable in current scope from value.
 * This operation takes ownership of the value. The value pointer must be 
 * allocated with ecs_value_new.
 */
FLECS_API
ecs_expr_var_t* ecs_vars_declare_w_value(
    ecs_vars_t *vars,
    const char *name,
    ecs_value_t *value);

/** Lookup variable in scope and parent scopes */
FLECS_API
ecs_expr_var_t* ecs_vars_lookup(
    const ecs_vars_t *vars,
    const char *name);

/** Used with ecs_parse_expr. */
typedef struct ecs_parse_expr_desc_t {
    const char *name;
    const char *expr;
    ecs_entity_t (*lookup_action)(
        const ecs_world_t*, 
        const char *value, 
        void *ctx);
    void *lookup_ctx;
    ecs_vars_t *vars;
} ecs_parse_expr_desc_t;

/** Parse expression into value.
 * This operation parses a flecs expression into the provided pointer. The
 * memory pointed to must be large enough to contain a value of the used type.
 * 
 * If no type and pointer are provided for the value argument, the operation 
 * will discover the type from the expression and allocate storage for the 
 * value. The allocated value must be freed with ecs_value_free.
 * 
 * @param world The world.
 * @param ptr The pointer to the expression to parse.
 * @param value The value containing type & pointer to write to.
 * @param desc Configuration parameters for deserializer.
 * @return Pointer to the character after the last one read, or NULL if failed.
 */
FLECS_API
const char* ecs_parse_expr(
    ecs_world_t *world,
    const char *ptr,
    ecs_value_t *value,
    const ecs_parse_expr_desc_t *desc);

/** Serialize value into expression string.
 * This operation serializes a value of the provided type to a string. The 
 * memory pointed to must be large enough to contain a value of the used type.
 * 
 * @param world The world.
 * @param type The type of the value to serialize.
 * @param data The value to serialize.
 * @return String with expression, or NULL if failed.
 */
FLECS_API
char* ecs_ptr_to_expr(
    const ecs_world_t *world,
    ecs_entity_t type,
    const void *data);

/** Serialize value into expression buffer.
 * Same as ecs_ptr_to_expr, but serializes to an ecs_strbuf_t instance.
 * 
 * @param world The world.
 * @param type The type of the value to serialize.
 * @param data The value to serialize.
 * @param buf The strbuf to append the string to.
 * @return Zero if success, non-zero if failed.
 */
FLECS_API
int ecs_ptr_to_expr_buf(
    const ecs_world_t *world,
    ecs_entity_t type,
    const void *data,
    ecs_strbuf_t *buf);

/** Similar as ecs_ptr_to_expr, but serializes values to string. 
 * Whereas the output of ecs_ptr_to_expr is a valid expression, the output of
 * ecs_ptr_to_str is a string representation of the value. In most cases the
 * output of the two operations is the same, but there are some differences:
 * - Strings are not quoted
 * 
 * @param world The world.
 * @param type The type of the value to serialize.
 * @param data The value to serialize.
 * @return String with result, or NULL if failed.
 */
FLECS_API
char* ecs_ptr_to_str(
    const ecs_world_t *world,
    ecs_entity_t type,
    const void *data);

/** Serialize value into string buffer.
 * Same as ecs_ptr_to_str, but serializes to an ecs_strbuf_t instance.
 * 
 * @param world The world.
 * @param type The type of the value to serialize.
 * @param data The value to serialize.
 * @param buf The strbuf to append the string to.
 * @return Zero if success, non-zero if failed.
 */
FLECS_API
int ecs_ptr_to_str_buf(
    const ecs_world_t *world,
    ecs_entity_t type,
    const void *data,
    ecs_strbuf_t *buf);

/** Serialize primitive value into string buffer.
 * Serializes a primitive value to an ecs_strbuf_t instance. This operation can
 * be reused by other serializers to avoid having to write boilerplate code that
 * serializes primitive values to a string.
 * 
 * @param world The world.
 * @param kind The kind of primitive value.
 * @param data The value ot serialize
 * @param buf The strbuf to append the string to.
 * @return Zero if success, non-zero if failed.
 */
FLECS_API
int ecs_primitive_to_expr_buf(
    const ecs_world_t *world,
    ecs_primitive_kind_t kind,
    const void *data, 
    ecs_strbuf_t *buf);

/** Parse expression token.
 * Expression tokens can contain more characters (such as '|') than tokens
 * parsed by the query (term) parser.
 * 
 * @param name The name of the expression (used for debug logs).
 * @param expr The full expression (used for debug logs).
 * @param ptr The pointer to the expression to parse.
 * @param token The buffer to write to (must have size ECS_MAX_TOKEN_SIZE)
 * @return Pointer to the character after the last one read, or NULL if failed.
 */
FLECS_API
const char *ecs_parse_expr_token(
    const char *name,
    const char *expr,
    const char *ptr,
    char *token);

/** Evaluate interpolated expressions in string.
 * This operation evaluates expressions in a string, and replaces them with
 * their evaluated result. Supported expression formats are:
 *  - $variable_name
 *  - {expression}
 * 
 * The $, { and } characters can be escaped with a backslash (\).
 * 
 * @param world The world.
 * @param str The string to evaluate.
 * @param vars The variables to use for evaluation.
 */
FLECS_API
char* ecs_interpolate_string(
    ecs_world_t *world,
    const char *str,
    const ecs_vars_t *vars);

/** Convert iterator to vars 
 * This operation converts an iterator to a variable array. This allows for
 * using iterator results in expressions. The operation only converts a 
 * single result at a time, and does not progress the iterator.
 * 
 * Iterator fields with data will be made available as variables with as name
 * the field index (e.g. "$1"). The operation does not check if reflection data
 * is registered for a field type. If no reflection data is registered for the
 * type, using the field variable in expressions will fail.
 * 
 * Field variables will only contain single elements, even if the iterator 
 * returns component arrays. The offset parameter can be used to specify which
 * element in the component arrays to return. The offset parameter must be
 * smaller than it->count.
 * 
 * The operation will create a variable for query variables that contain a
 * single entity.
 * 
 * The operation will attempt to use existing variables. If a variable does not
 * yet exist, the operation will create it. If an existing variable exists with
 * a mismatching type, the operation will fail.
 * 
 * Accessing variables after progressing the iterator or after the iterator is
 * destroyed will result in undefined behavior.
 * 
 * If vars contains a variable that is not present in the iterator, the variable
 * will not be modified.
 * 
 * @param it The iterator to convert to variables.
 * @param vars The variables to write to.
 * @param offset The offset to the current element.
 */
FLECS_API
void ecs_iter_to_vars(
    const ecs_iter_t *it,
    ecs_vars_t *vars,
    int offset);

/** @} */

#ifdef __cplusplus
}
#endif

#endif

#endif
