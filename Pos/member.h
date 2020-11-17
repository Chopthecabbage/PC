#ifndef MEMBER_H
#define MEMBER_H

#include "Common.h"

class Member
{
private:
    char MemberId[DefineMember::BUFFSIZE];          // 회원 아이디
    char MemberPassword[DefineMember::BUFFSIZE];    // 회원 비밀번호
    char MemberName[DefineMember::BUFFSIZE];        // 회원 이름
    char MemberBirth[DefineMember::BUFFSIZE];       // 생년월일
    char MemberPhone[DefineMember::BUFFSIZE];       // 연락처
    int MemberStatus;                 // 회원 상태 0 -> 빈 1-> 이용 2-> 이동
    char MemberRemain[DefineMember::BUFFSIZE];      // 잔여 시간
    int MemberSeat;                   // 좌석
    char MemberBegin[DefineMember::BUFFSIZE];       // 시작 시간
    int Membership; // 1:회원, 2:비회원
public:
    Member();
    Member(char* MemberId, char* MemberPassword, char* MemberName, char* MemberBirth, char* MemberPhone, int MemberStatus, char* MemberRemain, int MemberSeat, char* MemberBegin , int membership);
    void InitializeMember();
    //
    void Setting(char* MemberId, char* MemberPassword, char* MemberName, char* MemberBirth, char* MemberPhone, int MemberStatus, char* MemberRemain, int MemberSeat, char* MemberBegin, int membership);
    //
    char* GetMemberId();
    char* GetMemberPassword();
    char* GetMemberName();
    char* GetMemberBirth();
    char* GetMemberPhone();
    int GetMemberStatus() const;
    char* GetMemberRemain();
    int GetMemberSeat() const;
    char* GetMemberBegin();
    int GetMembership() const;

    void SetMemberId(char* MemberId);
    void SetMemberPassword(char* MemberPassword);
    void SetMemberName(char* MemberName);
    void SetMemberBirth(char* MemberBirth);
    void SetMemberPhone(char* MemberPhone);
    void SetMemberStatus(int MemberStatus);
    void SetMemberRemain(char* MemberRemain);
    void SetMemberSeat(int MemberSeat);
    void SetMemberBegin(char* MemberBegin);

    // 디버그
    void DebugShowMember() const;
};


#endif // MEMBER_H
