/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:44:14 by zamohame          #+#    #+#             */
/*   Updated: 2026/02/20 11:55:08 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Channel
{
    private:
    
    
    public:
        Channel();
        Channel(Channel const &src);
        Channel &operator=(Channel const &other);
        ~Channel();
};

#endif