/******************************************************************************
*
* Company       : Huizhou Desay SV Automotive Co., Ltd.
*
* Division      : Automotive Electronics, Desay Corporation
*
* Business Unit : Central Technology
*
* Department    : Advanced Development (Huizhou)
*
******************************************************************************/

#pragma once

#include "SVPType.h"
#include "SVPString.h"

/**
 * \brief Class to analyze argument from a named list
 * \details Argument Format: \--arg-name [arg-value]\n
 *          Argument Type: bool, int32_t, uint32_t, float, std::string
 */

template <typename T>
class SVPArgument {
  public:
    /**
     * \brief Construct an argument from argument list
     * \param[in] name Argument name
     * \param[in] default_value Default argument value, will be used when failed
     * \param[in] argc Size of named arguments list
     * \param[in] argv[] Named arguments list
     */
    SVPArgument(std::string name, T default_value,
                int32_t argc, const char* const argv[])
        :   _value(default_value)
        ,   _has(false) {
        int32_t _argPos = getArgumentPos(name, argc, argv);
        if (_argPos == -1)
            return;
        _has = true;

        int32_t _valuePos = _argPos + 1;
        if (_valuePos == argc || isArgument(argv[_valuePos]))
            return;
        transValue(argv[_valuePos], _value);

        int32_t _paramPos = _valuePos + 1;

        while (_paramPos < argc && !isArgument(argv[_paramPos])) {
            _params.push_back(argv[_paramPos++]);
        }
    };

    /**
     * \brief Get the argument value
     * \return The argument value
     * \sa has(), equal()
     */
    T get() {
        return _value;
    };

    /**
     * \brief Argument detection.
     * \return true if argument exist
     * \return false if argument not exist
     * \sa get(), equal()
     */
    bool has() {
        return _has;
    };

    /**
     * \brief Get Argument paramter count.
     * \return size of argument paramters
     * \sa getParam()
     */
    int32_t paramSize() {
        return _params.size();
    };

    /**
     * \brief Get the param value at idx
     * \return The param value
     * \sa paramSize()
     */
    template <typename P>
    P getParam(int32_t idx) {
        P _pValue = 0;
        if (idx < _params.size()) {
            transValue(_params[idx], _pValue);
        }
        return _pValue;
    }

    /**
     * \brief Argument value compare
     * \param value Value to compare with
     * \return true if argument value is the same as the input value
     * \return false if argument value is different with the input value
     * \sa get(), has()
     */
    bool equal(T value) {
        return _value == value;
    };

  private:
    inline int32_t getArgumentPos(std::string& name,
        int32_t argc, const char* const argv[]) {
        for (int32_t idx = 1; idx < argc; ++idx) {
            if ((argv[idx][0] == '-') && (name == &argv[idx][1])) {
                return idx;
            }
        }
        return -1;
    }

    inline bool isArgument(const char* arg) {
        return strlen(arg) > 2 && arg[0] == '-' && arg[1] == '-';
    }

    //void transValue(const char* var_str, T& value);
    void transValue(const char* var_str, bool& value) {
        value = atoi(var_str) ? 1 : 0;
    };

    void transValue(const char* var_str, int32_t& value) {
        value = atoi(var_str);
    };

    void transValue(const char* var_str, uint32_t& value) {
        value = atoll(var_str);
    };

    void transValue(const char* var_str, float& value) {
        value = atof(var_str);
    };

    void transValue(const char* var_str, std::string& value) {
        value = (var_str);
    };

  private:
    T _value;
    bool _has;
    std::vector<const char*> _params;
};
