#ifndef NOTE_H
#define NOTE_H

#include <string>

namespace cryptonotes {
    class Note {
    public:
        enum UpdateFlags {
            TITLE = 1,
            SUMMARY = 2,
            CONTENT = 4
        };

        Note();
        Note(long id, std::string title, std::string summary, std::string content, long timestamp);

        long id() const;
        std::string title() const;
        std::string summary() const;
        std::string content() const;

        void setTitle(std::string title);
        void setSummary(std::string summary);
        void setContent(std::string content);
        long timestamp() const;
    private:
        long _id;
        long _timestamp;
        std::string _title;
        std::string _summary;
        std::string _content;
    };
}

#endif
