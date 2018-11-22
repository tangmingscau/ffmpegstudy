package com.xiaopeng.ffmpegstudy.androidaudio;

import android.media.MediaPlayer;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.xiaopeng.ffmpegstudy.R;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * @author tony
 * @date 2018/11/22
 */
public class AudioAdapter extends RecyclerView.Adapter<AudioAdapter.Holder> {
    private List<File> mList = new ArrayList<>();
    MediaPlayer mPlayer;
    @Override
    public Holder onCreateViewHolder(ViewGroup parent, int viewType) {
        return new Holder(LayoutInflater.from(parent.getContext()).inflate(R.layout.item_audio, parent, false));
    }

    @Override
    public void onBindViewHolder(Holder holder, final int position) {
        holder.mTextView.setText(mList.get(position).getName());
        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    if (mPlayer!=null){
                        mPlayer.stop();
                        mPlayer.release();
                    }
                    mPlayer=new MediaPlayer();
                    mPlayer.setDataSource(mList.get(position).getPath());
                    mPlayer.prepare();
                    mPlayer.start();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    @Override
    public int getItemCount() {
        return mList.size();
    }
    public void setData(List<File> list){
        mList.clear();
        mList.addAll(list);
    }
    public static class Holder extends RecyclerView.ViewHolder {
        TextView mTextView;

        public Holder(View itemView) {
            super(itemView);
            mTextView = itemView.findViewById(R.id.title);
        }
    }
}
