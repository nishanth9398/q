/* Copyright (c) 2014 Jason Paulos
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef MATCHENGINE_H
#define MATCHENGINE_H

#include <map>
#include <QScopedPointer>
#include <QObject>
#include <QTimer>
#include "User.h"

/* This class will live on its own thread and match users there. It is designed to
 * only work with users of a specific gender so that two MatchEngines can be running
 * on different threads at the same time, hopefully maximizing efficientcy.
 *
 * At first I did not plan on using a QTimer to queue the match function. Instead I planned
 * on using a for loop, and using multiple threads would be necessary if that were the case.
 * However, because a QTimer is scheduling events, it is possible for the two MatchEngines
 * to operate on the GUI thread, avoiding the unnecessary hassle of multithreading. Depending
 * on the amount of users in the system, that could cause the program to slow down, or for
 * matching to take longer, so I'll continue to use multiple threads for now.
 */
class MatchEngine : public QObject{
    Q_OBJECT

public:
    explicit MatchEngine(std::vector<User> *input, const std::vector<User> *choices, int matchAmount);

public slots:
    void setup();
    void startMatching();
    void stopMatching();
    void match();

signals:
    void pleaseSetup();
    void pleaseStartMatching();
    void pleaseStopMatching();
    void progress(int completed);
    void done();

private:
    QScopedPointer<QTimer> timer;
    unsigned int index;
    std::vector<User> &input;
    const std::vector<User> &rawChoices;

    /* This map is cleared for every match and is then filled with the (squared) "distance" between the
     * current user from input and the potential matches from rawChoices. This way we can take advantage
     * of the sorting system in std::map because the potential matches who are the "closest" will occupy
     * the first positions in the map.
     */
    std::map<float, const User*> sortedChoices;

public:
    const unsigned int matchesPerSecond, matchAmount;
};

#endif // MATCHENGINE_H
