# ZIMPAF v1.2.1
- **Stability Fix**: utils/utils.c
    - function is_`zval_value_in_array`: Ensure that key is a string and not a remnants from previous request or from a garbage value.
    - function `concatenate_zval_array_into_string`: an explicit `IS_OBJECT` guard inside to prevent fatal casting exceptions (`Object of class stdClass could not be converted to string`) and allow the retrieval of nested.
    - function `get_func_param_string()`: an explicit `IS_OBJECT` guard inside  to prevent fatal casting exceptions (`Object of class stdClass could not be converted to string`)
- **Stability Fix**: libhooks/sanithook.c
    - function `generic_sanitation_handler`: add conditional statement to handle the type of sanitation functions parameter, an array or other type and call corresponding function to convert the parameter to string, `concatenate_zval_array_into_string` or `get_func_param_string`
- **Stability Fix**: libhooks/dirtravshook.c
    - add conditional statement to check for the number of arguments or parameters before retrieving the parameter.
- **Robustness**: Ensured uniform `200 OK` responses and seamless trace generation across multi-version PHP runtimes during complex Moodle fuzzing sessions.
