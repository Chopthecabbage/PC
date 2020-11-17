#include "member.h"

Member::Member()
{
    memset(MemberId,0,sizeof(MemberId));
    memset(MemberPassword,0,sizeof(MemberPassword));
    memset(MemberName,0,sizeof(MemberName));
    memset(MemberBirth,0,sizeof(MemberBirth));
    memset(MemberPhone,0,sizeof(MemberPhone));
    this->MemberStatus=0;
    memset(MemberRemain,0,sizeof(MemberRemain));
    this->MemberSeat=0;
    memset(MemberBegin,0,sizeof(MemberBegin));
}

Member::Member(char* MemberId, char* MemberPassword, char* MemberName, char* MemberBirth, char* MemberPhone, int MemberStatus, char* MemberRemain, int MemberSeat, char* MemberBegin)
{
    strcpy(this->MemberId,MemberId);
    strcpy(this->MemberPassword,MemberPassword);
    strcpy(this->MemberName,MemberName);
    strcpy(this->MemberBirth,MemberBirth);
    strcpy(this->MemberPhone,MemberPhone);
    this->MemberStatus=MemberStatus;
    strcpy(this->MemberRemain,MemberRemain);
    this->MemberSeat=MemberSeat;
    strcpy(this->MemberBegin,MemberBegin);
}

void Member::InitializeMember()
{
    memset(MemberId,0,sizeof(MemberId));
    memset(MemberPassword,0,sizeof(MemberPassword));
    memset(MemberName,0,sizeof(MemberName));
    memset(MemberBirth,0,sizeof(MemberBirth));
    memset(MemberPhone,0,sizeof(MemberPhone));
    this->MemberStatus=0;
    memset(MemberRemain,0,sizeof(MemberRemain));
    this->MemberSeat=0;
    memset(MemberBegin,0,sizeof(MemberBegin));
    this->Membership=0;
}

void Member::SettingNotSeatAndBegin(char* MemberId, char* MemberPassword, char* MemberName, char* MemberBirth, char* MemberPhone, int MemberStatus, char* MemberRemain)
{
    strcpy(this->MemberId,MemberId);
    strcpy(this->MemberPassword,MemberPassword);
    strcpy(this->MemberName,MemberName);
    strcpy(this->MemberBirth,MemberBirth);
    strcpy(this->MemberPhone,MemberPhone);
    this->MemberStatus=MemberStatus;
    strcpy(this->MemberRemain,MemberRemain);
}

void Member::Setting(char* MemberId, char* MemberPassword, char* MemberName, char* MemberBirth, char* MemberPhone, int MemberStatus, char* MemberRemain, int MemberSeat, char* MemberBegin)
{
    strcpy(this->MemberId,MemberId);
    strcpy(this->MemberPassword,MemberPassword);
    strcpy(this->MemberName,MemberName);
    strcpy(this->MemberBirth,MemberBirth);
    strcpy(this->MemberPhone,MemberPhone);
    this->MemberStatus=MemberStatus;
    strcpy(this->MemberRemain,MemberRemain);
    this->MemberSeat=MemberSeat;
    strcpy(this->MemberBegin,MemberBegin);
}

char* Member::GetMemberId()
{
    return MemberId;
}

char* Member::GetMemberPassword()
{
    return MemberPassword;
}

char* Member::GetMemberName()
{
    return MemberName;
}

char* Member::GetMemberBirth()
{
    return MemberBirth;
}

char* Member::GetMemberPhone()
{
    return MemberPhone;
}

int Member::GetMemberStatus() const
{
    return MemberStatus;
}

char* Member::GetMemberRemain()
{
    return MemberRemain;
}

int Member::GetMemberSeat() const
{
    return MemberSeat;
}

char* Member::GetMemberBegin()
{
    return MemberBegin;
}

int Member::GetMembership() const
{
    return Membership;
}

void Member::SetMemberId(char* MemberId)
{
    strcpy(this->MemberId,MemberId);
}

void Member::SetMemberPassword(char* MemberPassword)
{
    strcpy(this->MemberPassword,MemberPassword);
}

void Member::SetMemberName(char* MemberName)
{
    strcpy(this->MemberName,MemberName);
}

void Member::SetMemberBirth(char* MemberBirth)
{
    strcpy(this->MemberBirth,MemberBirth);
}

void Member::SetMemberPhone(char* MemberPhone)
{
    strcpy(this->MemberPhone,MemberPhone);
}

void Member::SetMemberStatus(int MemberStatus)
{
    this->MemberStatus=MemberStatus;
}

void Member::SetMemberRemain(char* MemberRemain)
{
    strcpy(this->MemberRemain,MemberRemain);
}

void Member::SetMemberSeat(int MemberSeat)
{
    this->MemberSeat=MemberSeat;
}

void Member::SetMemberBegin(char* MemberBegin)
{
    strcpy(this->MemberBegin,MemberBegin);
}

void Member::SetMembership(int Membership)
{
    this->Membership=Membership;
}

bool Member::operator==(const Member &m)
{
    if(strcmp(MemberId,m.MemberId)==0 && strcmp(MemberPassword,m.MemberPassword)==0 && strcmp(MemberName,m.MemberName)==0 && strcmp(MemberBirth,m.MemberBirth)==0 && strcmp(MemberPhone,m.MemberPhone)==0)
    {
        return true;
    }
    return false;
}

// 디버그
void Member::DebugShowMember() const
{
    qDebug() <<"아이디: "<<MemberId<<" 비밀번호: "<<MemberPassword<<" 이름: "<<MemberName<<" 생일: "<<MemberBirth<<" 연락처: "<<MemberPhone<<" 상태: "<<MemberStatus<<" 시간: "<<MemberRemain
             <<" 좌석: "<<MemberSeat<<" 시작시간: "<<MemberBegin<<" 유무: " <<Membership;
}
