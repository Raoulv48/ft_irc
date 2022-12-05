#ifndef DEBUG_HPP
#define DEBUG_HPP
#include <iostream>
#include <fstream>

# ifndef ENABLE_LOG
#  define ENABLE_LOG true
# endif

# ifndef ENABLE_DEBUG
#  define ENALBE_DEBUG true
# endif

# ifndef ENABLE_WARNING
#  define ENABLE_WARNING true
# endif

# ifndef ENABLE_ERROR
#  define ENABLE_ERROR true
# endif

# ifndef ENABLE_CRITICAL
#  define ENABLE_CRITICAL true
# endif

# ifndef ENABLE_COLORS
#  define ENABLE_COLORS true
# endif

# ifndef ENABLE_LOGGING
#  define ENABLE_LOGGING true
# endif

# if ENABLE_COLORS == true
#  define GREEN "\033[0;32m"
#  define RED "\033[0;31m"
#  define YELLOW "\033[0;33m"
#  define BLUE "\033[0;34m"
#  define RESET "\033[0m"
# else
#  define GREEN "\033[0m"
#  define RED "\033[0m"
#  define YELLOW "\033[0m"
#  define BLUE "\033[0m"
#  define RESET "\033[0m"
# endif

# if ENABLE_LOGGING == true
	static std::ofstream outlog;
# endif


#if ENABLE_LOGGING == false
# if ENABLE_LOG == true
#  define LOG(X) std::cout << "(LOG) " << X << __LINE__ << " in " << __FILE__ << std::endl;
# else
#  define LOG(X) ;
# endif

# if ENALBE_DEBUG == true
#  define DEBUG(X) std::cerr << GREEN << "(DEBUG) " << X << " at " << __LINE__ << " in " << __FILE__ << RESET << std::endl;
# else
#  define DEBUG(X) ;
# endif

# if ENABLE_WARNING == true
#  define WARNING(X) std::cerr << YELLOW << "(WARNING) " << X << " at " << __LINE__ << " in " << __FILE__ << RESET << std::endl;
# else
#  define WARNING(X) ;
# endif

# if ENABLE_ERROR == true
#  define ERROR(X) std::cerr << RED << "(ERROR) " << X << " at " << __LINE__ << " in " << __FILE__ << RESET <<std::endl;
# else
#  define ERROR(X) ;
# endif

# if ENABLE_CRITICAL == true
#  define CRITICAL(X) std::cerr << RED << "(CRITICAL_ERROR) " << X << " at " << __LINE__ << " in " << __FILE__ << RESET << std::endl; exit(1);
# else
#  define CRITICAL(X) ;
# endif
#endif

#if ENABLE_LOGGING == true
# if ENABLE_LOG == true
#  define LOG(X) std::cout << "(LOG) " << X << std::endl; outlog << "(LOG) " << __LINE__ << " in " << __FILE__ << X << std::endl;
# else
#  define LOG(X) ;
# endif

# if ENALBE_DEBUG == true
#  define DEBUG(X) std::cerr << GREEN << "(DEBUG) " << X << " at " << __LINE__ << " in " << __FILE__ << RESET << std::endl; outlog << "(DEBUG) " << X << " at " << __LINE__ << " in " << __FILE__  << std::endl;
# else
#  define DEBUG(X) ;
# endif

# if ENABLE_WARNING == true
#  define WARNING(X) std::cerr << YELLOW << "(WARNING) " << X << " at " << __LINE__ << " in " << __FILE__ << RESET << std::endl;  outlog << "(WARNING) " << X << " at " << __LINE__ << " in " << __FILE__  << std::endl;
# else
#  define WARNING(X) ;
# endif

# if ENABLE_ERROR == true
#  define ERROR(X) std::cerr << RED << "(ERROR) " << X << " at " << __LINE__ << " in " << __FILE__ << RESET <<std::endl;  outlog << "(ERROR) " << X << " at " << __LINE__ << " in " << __FILE__  << std::endl;
# else
#  define ERROR(X) ;
# endif

# if ENABLE_CRITICAL == true
#  define CRITICAL(X) std::cerr << RED << "(CRITICAL_ERROR) " << X << " at " << __LINE__ << " in " << __FILE__ << RESET << std::endl; exit(1);  outlog << "(CRITICAL_ERROR) " << X << " at " << __LINE__ << " in " << __FILE__  << std::endl;
# else
#  define CRITICAL(X) ;
# endif
#endif

#endif
